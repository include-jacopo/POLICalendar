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

map<string,string> IcalHandler::find_properties(icalcomponent* comp){
    icalproperty* p;

    vector<string> eventProperties; // vector che ospita le varie proprietà dell'evento
    map<string,string> eventProp;

    for(p = icalcomponent_get_first_property(comp, ICAL_ANY_PROPERTY); p != 0 ; p = icalcomponent_get_next_property(comp, ICAL_ANY_PROPERTY)){
        eventProp.insert({icalproperty_get_property_name(p),icalproperty_get_value_as_string(p)});
    }
    return eventProp;
}


Event IcalHandler::event_creator(map<string,string> eventProp, string etag){
    Event ev1;

    tm tm_start = {};
    tm tm_end = {};
    tm tm_creation = {};

    /* eseguo il parsing della stringa contenente la data */
    /* per il parsing ho guardato questo sito */
    /* https://www.ibm.com/docs/en/i/7.3?topic=functions-strptime-convert-string-datetime */
    strptime(   eventProp["DTSTART"].c_str(), "%Y%m%dT%H%M%SZ", &tm_start);
    strptime(   eventProp["DTEND"].c_str(), "%Y%m%dT%H%M%SZ", &tm_end);
    strptime(   eventProp["CREATED"].c_str(), "%Y%m%dT%H%M%SZ", &tm_creation);

    /**
     * Sostituito std::mktime con timegm.
     * std::mktime rileva la timezone e la data non risulta UTC come dovrebbe.
     *
     * timegm è non standard, per Windows è necessario utilizzare _mkgmtime.
     *
     * Alternativamente è possibile impostare temporaneamente la variabile TZ (timezone) dell'environment
     * a "UTC", ma anche questa istruzione potrebbe avere problemi di interoperabilità.
     *
     * See:
     * https://stackoverflow.com/questions/283166/easy-way-to-convert-a-struct-tm-expressed-in-utc-to-time-t-type/
     * https://stackoverflow.com/questions/530519/stdmktime-and-timezone-info
     * https://stackoverflow.com/questions/6467844/is-c-mktime-different-on-windows-and-gnu-linux
     * https://stackoverflow.com/questions/16647819/timegm-cross-platform
     *
     * Altre soluzioni riguardano l'uso di boost o C++20.
     * La std::chrono di C++20 tuttavia non è ancora completamente supportata da tutti i compilatori,
     * tra cui mingw su Windows (al 27/09/2021).
     */
    /* creo gli oggetti di tipo system_clock */
    auto tp_start = std::chrono::system_clock::from_time_t(timegm(&tm_start));
    auto tp_end = std::chrono::system_clock::from_time_t(timegm(&tm_end));
    auto tp_creation = std::chrono::system_clock::from_time_t(timegm(&tm_creation));


    tm tm_start1, tm_end1, tm_creation1;
    stringstream ss_start(eventProp["DTSTART"].c_str());


    /* print per capire se ho diviso bene le date */

    /*
   std::time_t tt1, tt2, tt3;
   tt1 = chrono::system_clock::to_time_t ( tp_start );
   tt2 = chrono::system_clock::to_time_t ( tp_end );
   tt3 = chrono::system_clock::to_time_t ( tp_creation );
   std::cout << "STart Time: "<< std::put_time(std::gmtime(&tt1), "%b %d %Y %H:%M:%S" ) <<endl<<
               "End Time: "<< std::put_time(std::gmtime(&tt2), "%b %d %Y %H:%M:%S" ) <<endl<<
             "Creation Time: "<< std::put_time(std::gmtime(&tt3), "%b %d %Y %H:%M:%S" ) <<endl;

     */

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
    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */
    //if(eventProp.find("ETAG")==eventProp.end()){
     //   etag= "";
    //}
    //else{
   //     etag = eventProp["ETAG"];
    //}

    Event ev = Event(eventProp["UID"], eventProp["SUMMARY"],description,location,url,etag,tp_creation, tp_start,tp_end);

    return ev;

};

Event IcalHandler::event_from_ical_component(icalcomponent* comp, string etag){
    map<string,string> eventProps;           /* mappa con il nome della proprietà come key e il valore della proprietà come value */
    eventProps = find_properties(comp);      /* estraggo le proprietà dal componente */

    Event ev = event_creator(eventProps, etag);   /* creo il nuovo evento passando le proprietà */

    return ev;

}

Task IcalHandler::task_creator(map<string,string> taskProp, string etag) {
    string description, location;
    bool flagData;                                                   /* flag che segnala la presenza della data */
    int priority;
    std::chrono::time_point<std::chrono::system_clock> tp_due, tp_stamp;       /* variabili che salva la data del task e la data di creazione del task *(

    /* converto la priorità da string a int */
    if (taskProp.find("PRIORITY") == taskProp.end()) {
        priority = 0;
    } else {
        /* leggo la priorità dalla mappa e la converto in int */
        priority = stoi(taskProp["PRIORITY"]);
    }
    //int priority_int = stoi(taskProp["PRIORITY"]);

    /* se non ho la proprietà due date => il task non avrà una data, altrimenti salvo la data e segnalo col flag la presenza della data */
    if (taskProp.find("DUE") != taskProp.end()) {
        tm tm_due = {};
        /* eseguo il parsing della stringa contenente la data */
        strptime(taskProp["DUE"].c_str(), "%Y%m%dT%H%M%SZ", &tm_due);
        /* creo gli oggetti di tipo system_clock */
        tp_due = std::chrono::system_clock::from_time_t(timegm(&tm_due));
        flagData = true;


    } else {

        flagData = false;
    }

    if (taskProp.find("DTSTAMP") != taskProp.end()) {
        tm tm_stamp = {};
        /* eseguo il parsing della stringa contenente la data */
        strptime(taskProp["DTSTAMP"].c_str(), "%Y%m%dT%H%M%SZ", &tm_stamp);
        /* creo gli oggetti di tipo system_clock */
        tp_stamp = std::chrono::system_clock::from_time_t(timegm(&tm_stamp));
    } else {

    }

    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */
    if (taskProp.find("DESCRIPTION") == taskProp.end()) {
        description = "";
    } else {
        description = taskProp["DESCRIPTION"];
    }

    if (taskProp.find("LOCATION") == taskProp.end()) {
        location = "";
    } else {
        location = taskProp["LOCATION"];
    }


    if (flagData == true) {
        /* chiamo il costruttore con data */
        Task t(taskProp["UID"],taskProp["SUMMARY"],description,location, etag, priority,0,tp_stamp,tp_due);
        //Task t{};
        return t;
    } else {

        /* chiamo il costruttore senza data */
        // TEMP Task t(taskProp["UID"], taskProp["SUMMARY"], "ciao", priority_int);
        Task t(taskProp["UID"],taskProp["SUMMARY"],description,location, etag, priority,0,tp_stamp);
        //Task t{};

        return t;

    }
}

Task IcalHandler::task_from_ical_component(icalcomponent* comp, string etag){
    map<string, string> taskProps;           /* mappa con il nome della proprietà come key e il valore della proprietà come value */

    taskProps = find_properties(comp);      /* estraggo le proprietà dal componente */

    /*
    for(auto i: taskProps){
        cout<<"key: "<<i.first<<" value: "<<i.second<<endl;
    }
     */

    Task t = task_creator(taskProps, etag);   /* creo il nuovo evento passando le proprietà */
    
    return t;
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
