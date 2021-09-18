#include <QApplication>
#include <QBoxLayout>
#include "Calendar.h"
#include "MainWindow.h"
#include <libical/ical.h>
#include <neon/ne_utils.h>
#include <iostream>
#include "backend/WebClient.h"
#include <pugixml.hpp>
#include <sstream>
#include "backend/XMLReader.h"
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
                        "SUMMARY:UFFAAAA\n"
                        "END:VEVENT\n"
                        "END:VCALENDAR";

    WebClient cal(host, user, pass, port);

    xml_cal = cal.report_calendar(uri_calendar);
    xml_todo = cal.report_todo(uri_todo);

    int prova = cal.put_event(uri_calendar, committami);

    list<icalcomponent*> eventi_calendario = readXML(xml_cal);
    list<icalcomponent*> todo_calendario = readXML(xml_todo);

    /*for(auto i: eventi_calendario){
        cout << icalcomponent_as_ical_string(i) << endl;
    }*/

    return 0;
}
