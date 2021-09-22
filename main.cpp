#include <QApplication>
#include <QBoxLayout>
#include "View/Calendar/Calendar.h"
#include "View/MainWindow.h"
#include <libical/ical.h>
#include <neon/ne_utils.h>
#include <iostream>
#include "backend/WebClient.h"
#include <pugixml.hpp>
#include <sstream>
#include "backend/XMLReader.h"
#include "backend/IcalHandler.h"

#include "Model/EventsContainer.h"
using namespace std;

int main(int argc, char *argv[]) {
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

    list<icalcomponent*> eventi_calendario = readXML(xml_cal);
    list<icalcomponent*> todo_calendario = readXML(xml_todo);

    EventsContainer contenitore_eventi;

    //Scorro ogni evento e i suoi sottoeventi per riempire Event.cpp
    for(auto evento: eventi_calendario){
        icalcomponent *c;
        for(c = icalcomponent_get_first_component(evento,ICAL_VEVENT_COMPONENT);
            c != 0;
            c = icalcomponent_get_next_component(evento,ICAL_VEVENT_COMPONENT)){
            //IcalHandler::find_properties(c); //Chiamata alla funzione per aggiungere in Event.cpp
            Event ev = IcalHandler::event_from_ical_component(c);
            contenitore_eventi.addEvent(ev);
        }
    }

    contenitore_eventi.displayEvents();


    return 0;

}
