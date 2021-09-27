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

Controller::Controller() : wc() {
    //DA SVUOTARE, QUESTO METODO LO DOVRÀ CHIAMARE RICCARDO
    createSession("dav.fruux.com", "b3297398995", "dap2zg5z54tu", 443);
}

void Controller::createSession (string url, string usr, string pw, int port){
    wc.setClient(url, usr, pw, port); //Autenticazione con il server
    wc.propfindUri(); //Riceve dal server gli url specifici per to-do e calendario
    wc.setCtag("PrimaLettura"); //Setto un ctag fittizio per la prima lettura

    downloadEvents(); //Riempio il calendario con gli eventi che già possiede
}

bool Controller::downloadEvents(){
    string ctagXML = wc.propfindCtag(wc.getUriCalendar()); //stringa ottenuta dalla PROPFIND
    string ctag = readCtag(ctagXML); //Estrazione del CTAG dalla stringa PROPFIND

    if(ctag != wc.getCtag() || ctag == "PrimaLettura") {
        wc.setCtag(ctag); //salvo nel wc il ctag corrispondente

        string xml_cal = wc.report_calendar(wc.getUriCalendar()); //Lettura dell'XML del calendario dal server
        string xml_todo = wc.report_todo(wc.getUriTodo()); //Lettura dell'XML dei to-do dal server

        list<icalcomponent *> eventi_calendario = readXML(xml_cal);
        list<icalcomponent *> todo_calendario = readXML(xml_todo);

        //Scorro ogni evento e i suoi sottoeventi per riempire Event.cpp
        for (auto evento: eventi_calendario) {
            icalcomponent *c;
            for (c = icalcomponent_get_first_component(evento, ICAL_VEVENT_COMPONENT);
                 c != 0;
                 c = icalcomponent_get_next_component(evento, ICAL_VEVENT_COMPONENT)) {
                        Event ev = IcalHandler::event_from_ical_component(c);
                        insertLocalEvent(ev);
            }
        }
        /* scorro la lista di componenti per creare gli oggetti task */
        /*
        cout<<"************ TASKS*****************"<<endl;
        for (auto todo: todo_calendario) {
            icalcomponent *c;
            for (c = icalcomponent_get_first_component(todo, ICAL_VTODO_COMPONENT);
                 c != 0;
                 c = icalcomponent_get_next_component(todo, ICAL_VTODO_COMPONENT)) {
                cout<<"componente che mando alla funzione !"<<endl;
                cout<<icalcomponent_as_ical_string(c)<<endl;
                cout<<"fine componente che mando alla funzione"<<endl;
                Task t = IcalHandler::task_from_ical_component(c);
                //out<<"ciao"<<endl;
               // insertLocalTask(t);
                //cout<<"uid: "<<t.getUid()<<" name: "<<t.getName()<<endl;
            }
        }
         */

        //CANCELLA DA QUI

        /*
        Event prova = getEvents().begin()->second;
        prova.setName("PROVA DI INSERIMENTO");
        prova.setUid("0d84aa00-bb6c-436b-af79-e1c79f0yt87f");
        //prova.setUid("0d84aa00bb6c436baf79e1c79f0yt87f");
        addEvent(prova); */
        for (auto i : Events){
            cout << "uid = " << i.first << " nome = " << i.second.getName() << endl;
        }
         cout << "NUOVI EVENTI" << endl;
        //Event prova = getEvents().at("7fe9b5cf-676f-4f77-8fa6-cc66b61ce4b3");

        deleteEvent("f6d63b21-a273-43d8-8bfa-a4ca26c9adab");

        auto it = Events.find("d38039f3-bb28-4ddb-a39d-f2211c7663c6");
        Event prova = it->second;
        prova.setName("Comprare lo smalto di Fedez");

        cout << prova.getUid() << endl;

        editEvent(prova);

        for (auto i : Events){
            cout << "uid = " << i.first << " nome = " << i.second.getName() << endl;
        }
        //CANCELLA FINO A QUI

        return true;

    } else {
        return false;
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
    //DA IMPLEMENTARE
    return false;
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
    streamStartT << std::put_time(std::localtime(&tt1), "%Y%m%dT%H%M%SZ" );
    streamEndT << std::put_time(std::localtime(&tt2), "%Y%m%dT%H%M%SZ" );
    streamCreationT << std::put_time(std::localtime(&tt3), "%Y%m%dT%H%M%SZ" );

    /* salvo lo stream di stringhe all'interno di una singola stringa */
    startT = streamStartT.str();
    endT = streamEndT.str();
    creationT = streamCreationT.str();

    string payloadIntermedio = "DTSTART:"+startT+"\n"+"UID:"+ev.getUid()+"\n"+"CREATED:"+creationT+"\n"+"DTSTAMP:"+creationT+"\n"+
                        "DTEND:"+endT+"\n"+"SUMMARY:"+ev.getName()+"\n";

    string payloadCompleto = payloadIniziale + payloadIntermedio + payloadFinale;

    if(!wc.put_event(wc.getUriCalendar()+ev.getUid(),payloadCompleto)){
        //La richiesta di caricamento dell'evento ha avuto risultato positivo, inserisco l'evento in locale
        Events.insert({ev.getUid(), ev});
        return  true;
    }
    else{
        return false;
    }
}

bool Controller::deleteEvent(string uid) {
    if(wc.deleteCalendar(uid)){ //se l'eliminazione online dell'evento è andata a buon fine
        auto it = Events.find(uid);
        if (it != Events.end()) {
            Events.erase(it); //rimuovo l'evento anche in locale
            return true;
        }
    } else {
        return false;
    }
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

    std::time_t tt1, tt2;
    /* ottengo degli oggetti time_t partendo dai campi chrono::system::clock dell'evento */
    tt1 = chrono::system_clock::to_time_t ( task.getDate());      /* data task */
    tt2 = chrono::system_clock::to_time_t ( task.getDateS());     /* data creazione task */


    string startT, dateT;
    stringstream streamStartT, streamDateT;

    /*inserisco l'output in uno stream di stringhe */
    streamDateT << std::put_time(std::localtime(&tt2), "%Y%m%dT%H%M%SZ" );


    /* salvo lo stream di stringhe all'interno di una singola stringa */

    dateT = streamDateT.str();

    /* aggiungo i campi obbligatori */
    string payloadIntermedio = "DTSTART:"+startT+"\n"+"UID:"+task.getUid()+"\n"+"DTSTAMP:"+dateT+"\n"+"\n"+"SUMMARY:"+task.getName()+"\n";
    /* aggiungo i campi opzionali */
    if(task.getDescription()!=""){
        payloadIntermedio = payloadIntermedio + "DESCRIPTION:"+task.getDescription()+"\n";
    }
    if(task.getLocation()!=""){
        payloadIntermedio = payloadIntermedio + "LOCATION:"+task.getLocation()+"\n";
    }
    if(task.isFlagDate()){
        tt1 = chrono::system_clock::to_time_t ( task.getDate());      /* data task */
        streamStartT << std::put_time(std::localtime(&tt1), "%Y%m%dT%H%M%SZ" );
        startT = streamStartT.str();
        payloadIntermedio = payloadIntermedio+"DTSTART:"+startT;
    }

    string payloadCompleto = payloadIniziale + payloadIntermedio + payloadFinale;

    if(!wc.put_event(wc.getUriTodo()+task.getUid(),payloadCompleto)){
        //La richiesta di caricamento dell'evento ha avuto risultato positivo, inserisco l'evento in locale
        Tasks.insert({task.getUid(), task});
        cout<<"HO INSERITO il task CORRETTAMENTe"<<endl;
        return  true;
    }
    else {
        cout << "INSERIMENTO FALLITO" << endl;
        return false;
    }
}

bool Controller::editTask(Task task) {
    // TODO
    return false;
}

bool Controller::deleteTask(string uid) {
    // TODO
    return false;
}
