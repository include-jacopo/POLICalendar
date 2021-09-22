//
// Created by Jacopo on 20/09/21.
//

#include "IcalHandler.h"
#include <libical/ical.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

map<string,string> IcalHandler::find_properties(icalcomponent* comp)
{
    icalproperty* p;

    vector<string> eventProperties;                 /* vector che ospita le varie proprietà dell'evento */
    map<string,string> eventProp;


    cout<<icalcomponent_as_ical_string(comp)<<endl;

    for(p = icalcomponent_get_first_property(comp, ICAL_ANY_PROPERTY); p != 0 ; p = icalcomponent_get_next_property(comp, ICAL_ANY_PROPERTY)){
        //string st = icalproperty_get_property_name(p);
        eventProp.insert({icalproperty_get_property_name(p),icalproperty_get_value_as_string(p)});
        //eventProperties.push_back(icalproperty_get_value_as_string(p));

    }

    for(auto i : eventProp){
        cout<<"Prop name: "<<i.first<<" Prop Value: "<<i.second<<endl;
        //cout<<"numero di parametri: "<<eventProp.size()<<endl;

    }
    event_creator(eventProp);

    return eventProp;
    /*
    if((p = icalcomponent_get_first_property(comp, ICAL_SUMMARY_PROPERTY))) {
        cout << icalproperty_get_value_as_string(p) << endl;
        //qui avviene l'aggiunta in Event.cpp per il summary
    }

    if((p = icalcomponent_get_first_property(comp, ICAL_LOCATION_PROPERTY))) {
        cout << icalproperty_get_value_as_string(p) << endl;
    }
     */

    //AGGIUNGERE TUTTI GLI IF NECESSARI A PESCARE TUTTI I DATI DI NOSTRO INTERESSE

}


Event IcalHandler::event_creator(map<string,string> eventProp){
    Event ev1;


    tm tm_start = {};
    tm tm_end = {};
    tm tm_creation = {};

    /* eseguo il parsing della stringa contenente la data */
    /* per il parsing ho guardato questo sito */
    /* https://www.ibm.com/docs/en/i/7.3?topic=functions-strptime-convert-string-datetime */
    strptime(   eventProp["DTSTART"].c_str(), "%Y%m%dT%H%M%S", &tm_start);
    strptime(   eventProp["DTSTART"].c_str(), "%Y%m%dT%H%M%S", &tm_end);
    strptime(   eventProp["DTSTART"].c_str(), "%Y%m%dT%H%M%S", &tm_creation);

    /* creo gli oggetti di tipo system_clock */
    auto tp_start = std::chrono::system_clock::from_time_t(std::mktime(&tm_start));
    auto tp_end = std::chrono::system_clock::from_time_t(std::mktime(&tm_end));
    auto tp_creation = std::chrono::system_clock::from_time_t(std::mktime(&tm_creation));


     // print per capire se ho diviso bene le date
    std::time_t tt1, tt2, tt3;
    tt1 = chrono::system_clock::to_time_t ( tp_start );
    tt2 = chrono::system_clock::to_time_t ( tp_end );
    tt3 = chrono::system_clock::to_time_t ( tp_creation );
    std::cout << "STart Time: "<< std::put_time(std::localtime(&tt1), "%Y %m %d  %H %M %S" ) <<
                " End Time: "<< std::put_time(std::localtime(&tt2), "%Y %m %d  %H %M %D" ) <<
              " Creation Time: "<< std::put_time(std::localtime(&tt3), "%Y %m %d  %H %M %D" ) <<endl;






    //cout<<"non funziona"<<endl;
    /*creo l'evento */
    string location, description, url;
    /* non ho la proprietà location, quindi passo una stringa vuota al costruttore */
    if(eventProp.find("LOCATION")==eventProp.end()){
        location = "";
    }
    else{
        location = eventProp["LOCATION"];
    }
    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */
    if(eventProp.find("DESCRIPTION")==eventProp.end()){
        description = "";
    }
    else{
        description = eventProp["DESCRIPTION"];
    }
    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */
    if(eventProp.find("URL")==eventProp.end()){
        url= "";
    }
    else{
        url = eventProp["URL"];
    }

    Event ev = Event(eventProp["UID"], eventProp["SUMMARY"],description,location,url,tp_creation, tp_start,tp_end);
    cout<<"UID: "<<ev.getUid()<<" NAME: "<<ev.getName()<<endl<<" DESCRIPTION: "<<ev.getDescription()<<" location: "<<ev.getLocation()<<" URL: "<<ev.getUrl()<<endl;
    return ev;



    /*
    if(eventProp.size()>6){
        cout<<"l'evento ha 7 campi!"<<endl;
        Event ev = Event(eventProp["UID"], eventProp["SUMMARY"],eventProp["DESCRIPTION"],eventProp["LOCATION"],tp_creation, tp_start,tp_end);
        cout<<"UID: "<<ev.getUid()<<" NAME: "<<ev.getName()<<endl;
        return ev;
    }
    if(eventProp.find("LOCATION")==eventProp.end()){
        /* non ho la proprietà location, quindi passo una stringa vuota al costruttore
        Event ev = Event(eventProp["UID"], eventProp["SUMMARY"],eventProp["DESCRIPTION"],"",tp_creation, tp_start,tp_end);
        return ev;
    } */




    /* evento con tutti i campi quindi chiamo il costruttore senza cercare il campo mancante */

    return ev1;

    // }
};

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
