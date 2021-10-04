//
// Created by michele on 9/22/21.
//

#include <map>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <type_traits>
#include <libical/ical.h>
#include "WebClient.h"
#include "XMLReader.h"
#include "IcalHandler.h"
#include "Controller.h"

using namespace std;

Controller *Controller::instance = nullptr;

Controller *Controller::getInstance() {
    if (instance == nullptr) {
        instance = new Controller();
    }
    return instance;
}

Controller::Controller() : wc() {}

int Controller::createSession (string url, string usr, string pw, int port){
    wc.setClient(url, usr, pw, port); //Autenticazione con il server

    switch (wc.tryLogin()) { //Funzione che testa il login e la connessione verso il server
        case 1:
            return 1; //Il login non è andato a buon fine
        case 2:
            return 2; //Errore di connessione con il server
        case 3:
            return 3; //Connessione timeout
        case 4:
            return 4; //Errore generico con il server
    }

    wc.propfindUri(); //Riceve dal server gli url specifici per to-do e calendario

    wc.setCtagCalendar("PrimaLettura"); //Setto un ctag fittizio per la prima lettura
    wc.setCtagTask("PrimaLettura");

    if(!downloadEvents()){ //Riempio il calendario con gli eventi che già possiede
        return 5; //in caso di errore
    }
    if(!downloadTask()) { //Riempio il calendario con i task che già possiede
        return 6; //in caso di errore
    }

    return 0; //la creazione della sessione è andata a buon fine
}

bool Controller::updateCtagCalendar() {
    string ctagXML;
    try {
        ctagXML = wc.propfindCtag(wc.getUriCalendar());
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    wc.setCtagCalendar(readCtag(ctagXML));
    return true; //l'aggiornamento del ctag è avvenuto correttamente
}

bool Controller::updateCtagTask() {
    string ctagXML;
    try {
        ctagXML = wc.propfindCtag(wc.getUriTask());
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    wc.setCtagTask(readCtag(ctagXML));
    return true; //l'aggiornamento del ctag è avvenuto correttamente
}

bool Controller::sync() {
    if(!updateEvents() || !updateTasks()){
        return false; // C'è stato un errore con il server
    }
    return true;
}

bool Controller::downloadEvents(){
    string old_ctag = wc.getCtagCalendar();

    if(!updateCtagCalendar()){
        return false; //c'è stato qualche problema nell'aggiornamento del ctag
    }

    if(old_ctag != wc.getCtagCalendar() || old_ctag == "PrimaLettura") {
        string xml_cal;
        try {
            xml_cal = wc.report_calendar(wc.getUriCalendar()); //Lettura dell'XML del calendario dal server
        } catch(invalid_argument &ie) {
            cout << ie.what() << endl;
            return false;
        }

        map<string,icalcomponent*> eventi_calendario = readXML(xml_cal);

        //Scorro ogni evento e i suoi sottoeventi per riempire Event.cpp
        for (auto evento: eventi_calendario) {
            icalcomponent *c;

            for (c = icalcomponent_get_first_component(evento.second, ICAL_VEVENT_COMPONENT); c != 0; c = icalcomponent_get_next_component(evento.second, ICAL_VEVENT_COMPONENT)) {
                        //Inserisco il componente nella nostra lista locale insieme al suo etag
                        Event ev = IcalHandler::event_from_ical_component(c, evento.first);
                        insertLocalEvent(ev);
            }
        }

        displayEvents();

        return true;

    } else {
        return false; //gli eventi sono già aggiornati
    }
}

bool Controller::downloadTask(){
    string old_ctag = wc.getCtagTask();

    if(!updateCtagTask()){
        return false; //c'è stato qualche problema nell'aggiornamento del ctag
    }

    if(old_ctag != wc.getCtagTask() || old_ctag == "PrimaLettura") {
        string xml_task;
        try {
            xml_task = wc.report_task(wc.getUriTask()); //Lettura dell'XML dei to-do dal server
        } catch(invalid_argument &ie) {
            cout << ie.what() << endl;
            return false;
        }

        map<string, icalcomponent*> task_calendario = readXML(xml_task);

        //Scorro la lista di componenti per creare gli oggetti task */
        for (auto task: task_calendario) {
            icalcomponent *c;
            for (c = icalcomponent_get_first_component(task.second, ICAL_VTODO_COMPONENT); c != 0; c = icalcomponent_get_next_component(task.second, ICAL_VTODO_COMPONENT)) {
                Task t = IcalHandler::task_from_ical_component(c, task.first);
                insertLocalTask(t);
            }
        }

        displayTasks();
        return true;

    } else {
        return false; //gli eventi sono già aggiornati
    }
}

const map<string, Event>& Controller::getEvents() {
    return Events;
}

int Controller::insertLocalEvent(Event ev){
    Events.insert({ev.getUid(), ev});
    return 1;
}

int Controller::insertLocalTask(Task t){
    Tasks.insert({t.getUid(), t});
    return 1;
}

bool Controller::updateEvents() {
    string old_ctag = wc.getCtagCalendar();

    if(!updateCtagCalendar()){
        return false; //errore di risposta dal server per ottenere il nuovo ctag
    }

    if(old_ctag != wc.getCtagCalendar()){
        //Leggo dalla richiesta l'elenco di tutti gli UID e ETAG per il calendario
        string etag_XML = wc.reportEtagCalendar();
        //Creo una mappa con in chiave l'UID e con valore l'ETAG per confrontarla con quelli che già ho
        map<string,string> eventi_con_etag = readEtagCalendar(etag_XML, wc.getUriCalendar());

        //Cerco se online sono presenti nuovi eventi oppure nuovi eventi modificati
        list<string> uid_nuovi_eventi; //Ospiterà i nuovi eventi trovati
        for(const auto& e: eventi_con_etag){
            auto pos_event = Events.find(e.first);
            if(pos_event != Events.end()){ //Trovato l'evento nella mappa locale
                //L'evento scaricato dal server già esiste in locale
                if(pos_event->second.getEtag() != e.second){ //controllo che l'etag sia differente
                    //cout << "UID ELEMENTO IN LISTA " << e.first << endl;
                    //cout << "ETAG ELEMENTO IN NOSTRA MAPPA " << pos_event->second.getEtag() << endl;
                    uid_nuovi_eventi.push_back(e.first); //lo aggiungo nella lista dei nuovi eventi da scaricare
                    Events.erase(pos_event); //Elimino la vecchia versione dalla lista locale
                }
            } else {
                //cout << "UID ELEMENTO NUOVO " << e.first << endl;
                uid_nuovi_eventi.push_back(e.first); //L'evento è nuovo per noi
            }
        }

        //Cerco se online sono stati cancellati degli eventi che noi abbiamo in locale
        for(const auto& e: Events) {
            auto pos_event = eventi_con_etag.find(e.first);
            if(pos_event == eventi_con_etag.end()){ //Se negli eventi online ho eliminato qualcosa che ho ancora in locale
                Events.erase(e.first); //la rimuovo dal locale
            }
        }

        if(uid_nuovi_eventi.size()>0){
            //Se ho qualche evento che è stato aggiunto o modificato
            string xml_cal;
            try {
                xml_cal = wc.multiGetCalendar(uid_nuovi_eventi); //Ottengo l'xml da una multiget utilizzando i nuovi uid
            } catch(invalid_argument &ie) {
                cout << ie.what() << endl;
                return false;
            }

            map<string,icalcomponent*> eventi_calendario = readXML(xml_cal);

            //Scorro ogni evento e i suoi sottoeventi per riempire Event.cpp
            for (auto evento: eventi_calendario) {
                icalcomponent *c;
                for (c = icalcomponent_get_first_component(evento.second, ICAL_VEVENT_COMPONENT); c != 0; c = icalcomponent_get_next_component(evento.second, ICAL_VEVENT_COMPONENT)) {
                    //Inserisco il componente nella nostra lista locale insieme al suo etag
                    Event ev = IcalHandler::event_from_ical_component(c, evento.first);
                    insertLocalEvent(ev);
                }
            }
        }
    }

    displayEvents();

    return true;
}

bool Controller::editEvent(Event ev) {
    if(deleteEvent(ev.getUid())){ //elimino prima l'evento vecchio
        if(addEvent(ev)){ //poi aggiungo l'evento modificato
            return true;
        }
    }
    return false;
}

bool Controller::addEvent(Event ev) {
    // Add event prima inserisce in remoto e poi successivamente in caso di inserzione con successo inserisce in locale
    // creo la stringa da passare alla funzione che manda la richiesta HTTP

    string payloadIniziale = "BEGIN:VCALENDAR\n"
                             "VERSION:2.0\n"
                             "PRODID:-//fruux//CalendarApp//EN\n"
                             "CALSCALE:GREGORIAN\n"
                             "BEGIN:VEVENT\n";

    string  payloadFinale =  "END:VEVENT\n"
                             "END:VCALENDAR";

    std::time_t tt1, tt2, tt3;
    /* ottengo degli oggetti time_t partendo dai campi chrono::system::clock dell'evento */
    tt1 = chrono::system_clock::to_time_t ( ev.getStartTime() );
    tt2 = chrono::system_clock::to_time_t ( ev.getEndTime() );
    tt3 = chrono::system_clock::to_time_t ( ev.getCreationTime() );

    string startT, endT, creationT;
    stringstream streamStartT, streamEndT, streamCreationT;

    /*inserisco l'output in uno stream di stringhe */
    streamStartT << std::put_time(std::gmtime(&tt1), "%Y%m%dT%H%M%SZ" );
    streamEndT << std::put_time(std::gmtime(&tt2), "%Y%m%dT%H%M%SZ" );
    streamCreationT << std::put_time(std::gmtime(&tt3), "%Y%m%dT%H%M%SZ" );

    /* salvo lo stream di stringhe all'interno di una singola stringa */
    startT = streamStartT.str();
    endT = streamEndT.str();
    creationT = streamCreationT.str();

    string payloadIntermedio = "DTSTART:"+startT+"\n"+"UID:"+ev.getUid()+"\n"+"CREATED:"+creationT+"\n"+"DTSTAMP:"+creationT+"\n"+
                               "DTEND:"+endT+"\n"+"SUMMARY:"+ev.getName()+"\n";

    string payloadCompleto = payloadIniziale + payloadIntermedio + payloadFinale;

    try {
        if(wc.put_event(wc.getUriCalendar()+ev.getUid(),payloadCompleto)){
            //La richiesta di caricamento dell'evento ha avuto risultato positivo, inserisco l'evento in locale
            Events.insert({ev.getUid(), ev});
            return  true;
        }
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    return false;
}

bool Controller::deleteEvent(string uid) {
    try {
        if (wc.deleteCalendar(uid)) { //se l'eliminazione online dell'evento è andata a buon fine
            auto it = Events.find(uid);
            if (it != Events.end()) {
                Events.erase(it); //rimuovo l'evento anche in locale
                return true;
            }
        }
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    return false;
}

optional<Event> Controller::findEvent(string uid) {

    auto it = Events.find(uid);

    if (it != Events.end()) {
        //Ho trovato l'evento e lo ritorno
        return it->second;
    }

    return {};

}

void Controller::displayEvents() {
    cout << "**EVENTI ATTUALMENTE PRESENTI NEL CONTENITORE**" << endl;
    for (auto i: Events) {
        i.second.printEvent();
    }
    cout << "***********************************************" << endl;
}

void Controller::displayTasks() {
    cout << "**EVENTI ATTUALMENTE PRESENTI NEL CONTENITORE**" << endl;
    for (auto i: Tasks) {
        i.second.printTask();
    }
    cout << "***********************************************" << endl;
}

const map<std::string, Task> &Controller::getTasks() {
    return Tasks;
}

bool Controller::addTask(Task task) {
    // Add event prima inserisce in remoto e poi successivamente in caso di inserzione con successo inserisce in locale
    // creo la stringa da passare alla funzione che manda la richiesta HTTP

    string payloadIniziale = "BEGIN:VCALENDAR\n"
                             "VERSION:2.0\n"
                             "CALSCALE:GREGORIAN\n"
                             "PRODID:-//fruux//CalendarApp//EN\n"
                             "BEGIN:VTODO\n";

    string  payloadFinale =  "END:VTODO\n"
                             "END:VCALENDAR";

    std::time_t tt1, tt2, tt3; //Otterro' degli oggetti time_t partendo dai campi chrono::system::clock dell'evento
    stringstream streamStartT, streamDateT, streamComplT; //Lo utilizzo per convertire l'oggetto di tipo time in stringa
    string startT, dateT, complT; //Per unire le stringhe dello stream in una stringa unica

   //Gestione della stringa di data creazione task
    tt2 = chrono::system_clock::to_time_t ( task.getDateS()); //Data creazione task
    streamDateT << std::put_time(std::gmtime(&tt2), "%Y%m%dT%H%M%SZ" );
    dateT = streamDateT.str(); //Salvo lo stream di stringhe all'interno di una singola stringa

    // Aggiungo i campi obbligatori
    string payloadIntermedio = "UID:"+task.getUid()+"\n"+"CREATED:"+dateT+"\n"+"DTSTAMP:"+dateT;
            //+"\n"+"DTSTAMP:"+dateT+"\n"+"\n"+"SUMMARY:"+task.getName()+"\n";

    //Aggiungo se il task è completato o deve esserlo
    if(task.isCompleted()){
        payloadIntermedio = payloadIntermedio+"\n"+"STATUS:COMPLETED";
    } else {
        payloadIntermedio = payloadIntermedio+"\n"+"STATUS:NEEDS-ACTION";
    }

    //Se c'è la descrizione la aggiungo
    if(!task.getDescription().empty()){
        payloadIntermedio = payloadIntermedio+"\n"+"DESCRIPTION:"+task.getDescription();
    }

    //Se c'è una data di scadenza la aggiungo
    if(task.isFlagDate()){
        tt1 = chrono::system_clock::to_time_t (task.getDueDate()); //Data scadenza task
        streamStartT << std::put_time(std::gmtime(&tt1), "%Y%m%dT%H%M%SZ" );
        startT = streamStartT.str();
        payloadIntermedio = payloadIntermedio+"\n"+"DUE:"+startT;
    }

    //Aggiungo il tipo di prirità al task se esiste (1 bassa, 3 media, 5 alta)
    if(task.getPriority() > 0) {
        payloadIntermedio = payloadIntermedio + "\n" + "PRIORITY:" + to_string(task.getPriority());
    }

    //Il summary dell'evento è obbligatorio
    payloadIntermedio = payloadIntermedio+"\n"+"SUMMARY:"+task.getName();

    //Aggiungo la data di quando è stato completato il task
    if(task.isCompleted()){
        tt3 = chrono::system_clock::to_time_t (task.getDateCompleted());      /* data task */
        streamComplT << std::put_time(std::gmtime(&tt3), "%Y%m%dT%H%M%SZ" );
        complT = streamComplT.str();
        cout << "L'ho completato il: " + complT << endl;
        payloadIntermedio = payloadIntermedio+"\n"+"COMPLETED:"+complT;

    }

    //Aggiungo l'eventuale location
    if(!task.getLocation().empty()){
        payloadIntermedio = payloadIntermedio+"\n"+"LOCATION:"+task.getLocation();
    }

    string payloadCompleto = payloadIniziale + payloadIntermedio + "\n" + payloadFinale;

    cout << payloadCompleto << endl;

    try {
        if(wc.put_event(wc.getUriTask()+task.getUid(),payloadCompleto)){
            //La richiesta di caricamento dell'evento ha avuto risultato positivo, inserisco il task in locale
            Tasks.insert({task.getUid(), task});
            return  true;
        }
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    return false;
}

bool Controller::editTask(Task task) {
    if(deleteTask(task.getUid())){ //elimino prima l'evento vecchio
        if(addTask(task)){ //poi aggiungo l'evento modificato
            return true;
        }
    }
    return false;
}

bool Controller::deleteTask(string uid) {
    try {
        if (wc.deleteTask(uid)) { //se l'eliminazione online dell'evento è andata a buon fine
            auto it = Tasks.find(uid);
            if (it != Tasks.end()) {
                Tasks.erase(it); //rimuovo l'evento anche in locale
                cout<<"ho rimosso con successo il task"<<endl;
                return true;
            }
        }
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    return false;
}

bool Controller::updateTasks() {
    string old_ctag = wc.getCtagTask();

    if(!updateCtagTask()) {
        return false; //Errore con il server nell'ottenere il ctag
    }

    if(old_ctag != wc.getCtagTask()){ //Solo se il ctag è cambiato provo a leggere i nuovi
        //Leggo dalla richiesta l'elenco di tutti gli UID e ETAG per il calendario
        string etag_XML = wc.reportEtagTask();
        //Creo una mappa con in chiave l'UID e con valore l'ETAG per confrontarla con quelli che già ho
        map<string,string> task_con_etag = readEtagTask(etag_XML, wc.getUriTask());

        list<string> uid_nuovi_task; //Ospiterà i nuovi task trovati
        for(const auto& t: task_con_etag){
            auto pos_task = Tasks.find(t.first);
            if(pos_task != Tasks.end()){ //Trovato il task nella mappa locale
                //Il task scaricato dal server già esiste in locale
                if(pos_task->second.getEtag() != t.second){ //controllo che l'etag sia differente
                    //cout << "UID ELEMENTO IN LISTA " << t.first << endl;
                    //cout << "ETAG ELEMENTO IN NOSTRA MAPPA " << pos_event->second.getEtag() << endl;
                    uid_nuovi_task.push_back(t.first); //lo aggiungo nella lista dei nuovi eventi da scaricare
                    Tasks.erase(pos_task); //Elimino la vecchia versione dalla lista locale
                }
            } else {
                //cout << "UID ELEMENTO NUOVO " << e.first << endl;
                uid_nuovi_task.push_back(t.first); //L'evento è nuovo per noi
            }
        }

        //Cerco se online sono stati cancellati dei task che noi abbiamo in locale
        for(const auto& t: Tasks) {
            auto pos_task = task_con_etag.find(t.first);
            if(pos_task == task_con_etag.end()){ //Se negli eventi online ho eliminato qualcosa che ho ancora in locale
                Tasks.erase(t.first); //la rimuovo dal locale
            }
        }

        if(uid_nuovi_task.size() > 0){
            //Se ho qualche evento che è stato aggiunto o modificato
            string xml_task;
            try {
                xml_task = wc.multiGetTask(uid_nuovi_task); //Ottengo l'xml da una multiget utilizzando i nuovi uid
            } catch(invalid_argument &ie) {
                cout << ie.what() << endl;
                return false;
            }

            map<string,icalcomponent*> task_calendario = readXML(xml_task);

            //Scorro ogni evento e i suoi sottoeventi per riempire Event.cpp
            for (auto task: task_calendario) {
                icalcomponent *c;
                for (c = icalcomponent_get_first_component(task.second, ICAL_VTODO_COMPONENT); c != 0; c = icalcomponent_get_next_component(task.second, ICAL_VTODO_COMPONENT)) {
                    Task t = IcalHandler::task_from_ical_component(c, task.first);
                    insertLocalTask(t);
                }
            }
        }
    }

    displayTasks();

    return true;
}
