//
// Created by michele on 9/22/21.
//

#include <map>
#include <string>
#include <iostream>
#include <libical/ical.h>
#include "WebClient.h"
#include "XMLReader.h"
#include "IcalHandler.h"
#include "Controller.h"

#include <chrono>
#include <ctime>
#include <iomanip>

#include <sstream>


#include <type_traits>

using namespace std;

Controller::Controller(): wc("dav.fruux.com","b3297398995","dap2zg5z54tu",443) {
    const string host("dav.fruux.com");
    const string user("b3297398995");
    const string pass("dap2zg5z54tu");
    const string uri_calendar("/calendars/a3298160768/51759490-6b14-4c41-88ae-1a94106fe0b6/");
    const string uri_todo("/calendars/a3298160768/4e84299f-0505-4cbb-8007-c29808fe25b6/");
    const unsigned port = 443; //443
    string xml_cal;
    string xml_todo;

    //wc = WebClient(host, user, pass, port);

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



    xml_cal = wc.report_calendar(uri_calendar);
    xml_todo = wc.report_todo(uri_todo);

    //ESEMPIO DI AGGIUNTA DI UN EVENTO
    //int prova = cal.put_event(uri_calendar, committami);

    list<icalcomponent *> eventi_calendario = readXML(xml_cal);
    list<icalcomponent *> todo_calendario = readXML(xml_todo);

    //Scorro ogni evento e i suoi sottoeventi per riempire Event.cpp
    for (auto evento: eventi_calendario) {
        icalcomponent *c;
        for (c = icalcomponent_get_first_component(evento, ICAL_VEVENT_COMPONENT);
             c != 0;
             c = icalcomponent_get_next_component(evento, ICAL_VEVENT_COMPONENT)) {
            //IcalHandler::find_properties(c); //Chiamata alla funzione per aggiungere in Event.cpp
            Event ev = IcalHandler::event_from_ical_component(c);
            addEvent(ev);
        }
    }
    Event ev2 = Events.find("0d84aa00-bb6c-436b-af79-e1c79f0fb87f")->second;
    cout<<"EVENTO 2!!"<<endl<<"descrizione" << ev2.getDescription()<<endl;
    ev2.setDescription("MICHELE NON PUZZA!!, JACOPO PUZZA");
    cout<<"EVENTO 2!!"<<endl<<"descrizione" << ev2.getDescription()<<endl;
    addEvent(ev2);
};

pair<forward_iterator_tag, forward_iterator_tag> Controller::getEvents() {
    return pair<forward_iterator_tag, forward_iterator_tag>();
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
    string  payloadFinale = "END:VEVENT\n"
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
    cout<<startT<<" questo è il startT!"<<endl;
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
                        "DTEND"+endT+"\n"+"SUMMARY:"+ev.getName();

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

void Controller::setView(IView *view) {
    this->view = view;
}
