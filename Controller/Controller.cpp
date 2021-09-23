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

using namespace std;

Controller* Controller::instance = nullptr;

Controller* Controller::getInstance() {
    if (instance == nullptr) {
        instance = new Controller();
    }
    return instance;
}

Controller::Controller() {
    const string host("dav.fruux.com");
    const string user("b3297398995");
    const string pass("dap2zg5z54tu");
    const string uri_calendar("/calendars/a3298160768/51759490-6b14-4c41-88ae-1a94106fe0b6/");
    const string uri_todo("/calendars/a3298160768/4e84299f-0505-4cbb-8007-c29808fe25b6/");
    const unsigned port = 443; //443
    string xml_cal;
    string xml_todo;

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

    WebClient cal(host, user, pass, port);

    xml_cal = cal.report_calendar(uri_calendar);
    xml_todo = cal.report_todo(uri_todo);

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
};

const map<string, Event>& Controller::getEvents() {
    return Events;
}

bool Controller::updateEvents() {
    return false;
}

bool Controller::addEvent(Event ev) {
    Events.insert({ev.getUid(), ev});
    return true;
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
