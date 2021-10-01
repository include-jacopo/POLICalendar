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

bool Controller::createSession (string url, string usr, string pw, int port){
    wc.setClient(url, usr, pw, port); //Autenticazione con il server
    try {
        wc.propfindUri(); //Riceve dal server gli url specifici per to-do e calendario
    } catch(invalid_argument &ie) {
        cout << ie.what() << endl;
        return false;
    }
    wc.setCtagCalendar("PrimaLettura"); //Setto un ctag fittizio per la prima lettura
    wc.setCtagTask("PrimaLettura");
    downloadEvents(); //Riempio il calendario con gli eventi che già possiede
    downloadTask(); //Riempio il calendario con gli eventi che già possiede

    return true; //la creazione della sessione è andata a buon fine
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
    // TODO!
    return false;
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

            for (c = icalcomponent_get_first_component(evento.second, ICAL_VEVENT_COMPONENT);
                 c != 0;
                 c = icalcomponent_get_next_component(evento.second, ICAL_VEVENT_COMPONENT)) {
                        //Inserisco il componente nella nostra lista locale insieme al suo etag
                        Event ev = IcalHandler::event_from_ical_component(c, evento.first);
                        insertLocalEvent(ev);
            }
        }

        displayEvents();

        /*
        Task tprova;
        tprova = Tasks.begin()->second;

        tprova.setDateCompleted(Tasks.begin()->second.getDateS());
        tprova.setName("PROVA TASK COMPLETED 5");
        tprova.setUidS("ciaoUID123452246");
        tprova.setCompleted(true);
        addTask(tprova);

        */
        /*
        Task prova = getTasks().begin()->second;
        prova.setUidS("stringauid12345");
        prova.setName("task inserito a mano");
        auto prova4 = new Task;
        cout<<"task creato senza parametri"<<" uid: "<<prova4->getUid()<<endl;
        //cout<<prova.getUid()<<endl;
        addTask(*prova4);
*/
        string uno = "CICCIOGAMER";
        string due = "CICCIO";

        updateEvents();
        updateTasks();
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
            for (c = icalcomponent_get_first_component(task.second, ICAL_VTODO_COMPONENT);
                 c != 0;
                 c = icalcomponent_get_next_component(task.second, ICAL_VTODO_COMPONENT)) {

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

    /* QUESTO PEZZO DI CODICE CONTROLLA CHE IL CTAG SIA CAMBIATO. ELIMINA SOLO LE /* DOPO CHE L'HAI IMPLEMENTATA
    if(!updateCtag() || old_ctag == wc.getCtag()){
        //Se qualcosa è andato storto nell'aggiornamento del ctag o il ctag non è cambiato
        return false;
    } */

    //Leggo dalla richiesta l'elenco di tutti gli UID e ETAG per il calendario
    string etag_XML = wc.reportEtagCalendar();
    //Creo una mappa con in chiave l'UID e con valore l'ETAG per confrontarla con quelli che già ho
    map<string,string> eventi_con_etag = readEtagCalendar(etag_XML, wc.getUriCalendar());

    //DA IMPLEMENTARE
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
                             "X-WR-CALNAME:Calendar\n"
                             "X-APPLE-CALENDAR-COLOR:#B90E28\n"
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
                             "PRODID:-//fruux//CalendarApp//EN\n"
                             "CALSCALE:GREGORIAN\n"
                             "X-WR-CALNAME:Calendar\n"
                             "X-APPLE-CALENDAR-COLOR:#B90E28\n"
                             "BEGIN:VTODO\n";

    string  payloadFinale =  "END:VTODO\n"
                             "END:VCALENDAR";

    std::time_t tt1, tt2, tt3;
    /* ottengo degli oggetti time_t partendo dai campi chrono::system::clock dell'evento */
    tt1 = chrono::system_clock::to_time_t (task.getDueDate());      //Data task
    tt2 = chrono::system_clock::to_time_t ( task.getDateS());     //Data creazione task


    string startT, dateT, complT;
    stringstream streamStartT, streamDateT, streamComplT;

    /* inserisco l'output in uno stream di stringhe */
    streamDateT << std::put_time(std::gmtime(&tt2), "%Y%m%dT%H%M%SZ" );


    /* salvo lo stream di stringhe all'interno di una singola stringa */

    dateT = streamDateT.str();

    /* aggiungo i campi obbligatori */
    string payloadIntermedio = "DTSTART:"+startT+"\n"+"UID:"+task.getUid()+"\n"+"DTSTAMP:"+dateT+"\n"+"\n"+"SUMMARY:"+task.getName()+"\n";
    /* aggiungo i campi opzionali */
    if(!task.getDescription().empty()){
        payloadIntermedio = payloadIntermedio + "DESCRIPTION:"+task.getDescription()+"\n";
    }
    if(!task.getLocation().empty()){
        payloadIntermedio = payloadIntermedio + "LOCATION:"+task.getLocation()+"\n";
    }
    if(task.isFlagDate()){
        tt1 = chrono::system_clock::to_time_t (task.getDueDate());      /* data task */
        streamStartT << std::put_time(std::gmtime(&tt1), "%Y%m%dT%H%M%SZ" );
        startT = streamStartT.str();
        payloadIntermedio = payloadIntermedio+"DTSTART:"+startT;
    }
    if(task.isCompleted()){
        tt3 = chrono::system_clock::to_time_t (task.getDateCompleted());      /* data task */
        streamComplT << std::put_time(std::gmtime(&tt3), "%Y%m%dT%H%M%SZ" );
        complT = streamStartT.str();
        payloadIntermedio = payloadIntermedio+"COMPLETED:"+complT+"\n";

    }

    string payloadCompleto = payloadIniziale + payloadIntermedio + payloadFinale;

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

    /* QUESTO PEZZO DI CODICE CONTROLLA CHE IL CTAG SIA CAMBIATO. ELIMINA SOLO LE /* DOPO CHE L'HAI IMPLEMENTATA
    if(!updateCtagTask() || old_ctag == wc.getCtagTask()){
        //Se qualcosa è andato storto nell'aggiornamento del ctag o il ctag non è cambiato
        return false;
    } */

    //Leggo dalla richiesta l'elenco di tutti gli UID e ETAG per il calendario
    string etag_XML = wc.reportEtagTask();
    //Creo una mappa con in chiave l'UID e con valore l'ETAG per confrontarla con quelli che già ho
    map<string,string> task_con_etag = readEtagTask(etag_XML, wc.getUriTask());

    //DA IMPLEMENTARE
    return true;
}
