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
    wc.setUri(); //Riceve dal server gli url specifici per to-do e calendario
    wc.setCtag("PrimaLettura"); //Setto un ctag fittizio per la prima lettura
    downloadEvents(); //Riempio il calendario con gli eventi che già possiede
}

bool Controller::downloadEvents(){
    string ctagXML = wc.propfind_calendar(wc.getUriCalendar()); //stringa ottenuta dalla PROPFIND
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
                cout<<"uid: "<<ev.getUid()<<" name: "<<ev.getName()<<endl;
            }
        }

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

bool Controller::updateEvents() {
    return false;
}

bool Controller::addEvent(Event ev) {
    /*add event prima inserisce in remoto e poi successivamente in caso di inserzione con successo inserisce in locale */
    /* creo la stringa da passare alla funzione che manda la richiesta HTTP */

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

    /* ESEMPIO DI PAYLOAD DA CREARE
   string committami = "BEGIN:VCALENDAR\n"
                       "VERSION:2.0\n"
                       "PRODID:-//fruux//CalendarApp//EN\n"
                       "CALSCALE:GREGORIAN\n"
                       "X-WR-CALNAME:Calendar\n"
                       "X-APPLE-CALENDAR-COLOR:#B90E28\n"
                       "BEGIN:VEVENT\n"
                       "DTSTART:20210930T150000Z\n"
                       "UID:ec137329-0a8b-41d4-9ec8-0b886b8df13a\n"
                       "CREATED:20210917T142720Z\n"
                       "DTSTAMP:20210917T142734Z\n"
                       "DTEND:20210930T170000Z\n"
                       "SUMMARY:STUPIRE RICCARDO\n"
                       "END:VEVENT\n"
                       "END:VCALENDAR";
    */
    string payloadIntermedio = "DTSTART:"+startT+"\n"+"UID:"+ev.getUid()+"\n"+"CREATED:"+creationT+"\n"+"DTSTAMP:"+creationT+"\n"+
                        "DTEND:"+endT+"\n"+"SUMMARY:"+ev.getName()+"\n";

    string payloadCompleto = payloadIniziale + payloadIntermedio + payloadFinale;


    if(!wc.put_event("/calendars/a3298160768/51759490-6b14-4c41-88ae-1a94106fe0b6/",payloadCompleto)){
        /* la richiesta di caricamento dell'evento ha avuto risultato positivo */
        /* inserisco l'evento in locale */
        Events.insert({ev.getUid(), ev});
        cout<<"HO INSERITO GLI EVENTI CORRETTAMENTI"<<endl;
        return  true;
    }
    else{
        cout<<"INSERIMENTO FALLITO"<<endl;
        return false;
    }
}

bool Controller::deleteEvent(string uid) {

    auto it = Events.find(uid);

    if (it != Events.end()) {
        /* ho trovato l'evento da rimuovere */
        Events.erase(it);
        return true;
    }
    return false;

    /*
     * vecchia implementazione con il vector
    vector<Event>::iterator it;
    Event removed;
    it = remove_if(Events.begin(), Events.end(),[uid, &removed](Event ev){
                                if(ev.getUid()==uid) {
                                    removed = ev;
                                    return 1;
                                }
                                else
                                    return 0;} );
    if( it!= Events.end())
        return removed;
    */
}

optional<Event> Controller::findEvent(string uid) {

    auto it = Events.find(uid);

    if (it != Events.end()) {
        /* ho trovato l'evento e lo ritorno */
        return it->second;
    }

    return {};

    /*
     *  vecchia implementazione con vector

    vector<Event>::iterator it;
    Event found;
    it = find(Events.begin(), Events.end(),[uid, &found](Event ev){
        if(ev.getUid()==uid) {
            found = ev;
            return 1;
        }
        else
            return 0;} );
    if( it!= Events.end())
        return removed;
    */
}

void Controller::displayEvents() {
    cout << "**EVENTI ATTUALMENTE PRESENTI NEL CONTENITORE**" << endl;
    for (auto i: Events) {
        i.second.printEvent();
    }
    cout << "***********************************************" << endl;
}
