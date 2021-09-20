//
// Created by Jacopo on 20/09/21.
//

#include "IcalHandler.h"
#include <libical/ical.h>
#include <libical/icalenums.h>
#include <iostream>
using namespace std;

void IcalHandler::find_properties(icalcomponent* comp)
{
    icalproperty* p;

    if((p = icalcomponent_get_first_property(comp, ICAL_SUMMARY_PROPERTY))) {
        cout << icalproperty_get_value_as_string(p) << endl;
        //qui avviene l'aggiunta in Event.cpp per il summary
    }

    if((p = icalcomponent_get_first_property(comp, ICAL_LOCATION_PROPERTY))) {
        cout << icalproperty_get_value_as_string(p) << endl;
    }

    //AGGIUNGERE TUTTI GLI IF NECESSARI A PESCARE TUTTI I DATI DI NOSTRO INTERESSE

}

//ESEMPIO DI EVENTO PIU' COMPLETO
/* BEGIN:VCALENDAR
VERSION:2.0
PRODID:-//fruux//CalendarApp//EN
CALSCALE:GREGORIAN
X-WR-CALNAME:Calendar
X-APPLE-CALENDAR-COLOR:#B90E28
BEGIN:VEVENT
DTSTART:20210921T150000Z
UID:0d84aa00-bb6c-436b-af79-e1c79f0fb87f
CREATED:20210918T145053Z
DTSTAMP:20210918T145151Z
DTEND:20210921T170000Z
DESCRIPTION:Michele puzza :(
LOCATION:casa di Riccardo a Bologna
SUMMARY:Comprare iPhone 13
URL:www.google.com
BEGIN:VALARM
UID:6d569dbf-879c-4a25-a6bd-1db1e1ed9f8f
ACTION:DISPLAY
DESCRIPTION:Event reminder
TRIGGER:-PT5M
END:VALARM
BEGIN:VALARM
UID:59f5b244-506a-4309-8a49-10a249b3bd8a
ACTION:DISPLAY
DESCRIPTION:Event reminder
TRIGGER:-PT30M
END:VALARM
END:VEVENT
END:VCALENDAR*/
