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


    //cout<<icalcomponent_as_ical_string(comp)<<endl;

    for(p = icalcomponent_get_first_property(comp, ICAL_ANY_PROPERTY); p != 0 ; p = icalcomponent_get_next_property(comp, ICAL_ANY_PROPERTY)){
        //string st = icalproperty_get_property_name(p);
        eventProp.insert({icalproperty_get_property_name(p),icalproperty_get_value_as_string(p)});
        //eventProperties.push_back(icalproperty_get_value_as_string(p));

    }

    //event_creator(eventProp);

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
    strptime(   eventProp["DTSTART"].c_str(), "%Y%m%dT%H%M%SZ", &tm_start);
    strptime(   eventProp["DTEND"].c_str(), "%Y%m%dT%H%M%SZ", &tm_end);
    strptime(   eventProp["CREATED"].c_str(), "%Y%m%dT%H%M%SZ", &tm_creation);

    /* creo gli oggetti di tipo system_clock */
    auto tp_start = std::chrono::system_clock::from_time_t(std::mktime(&tm_start));
    auto tp_end = std::chrono::system_clock::from_time_t(std::mktime(&tm_end));
    auto tp_creation = std::chrono::system_clock::from_time_t(std::mktime(&tm_creation));


    tm tm_start1, tm_end1, tm_creation1;
    stringstream ss_start(eventProp["DTSTART"].c_str());


     /* print per capire se ho diviso bene le date */

     /*
    std::time_t tt1, tt2, tt3;
    tt1 = chrono::system_clock::to_time_t ( tp_start );
    tt2 = chrono::system_clock::to_time_t ( tp_end );
    tt3 = chrono::system_clock::to_time_t ( tp_creation );
    std::cout << "STart Time: "<< std::put_time(std::localtime(&tt1), "%b %d %Y %H:%M:%S" ) <<endl<<
                "End Time: "<< std::put_time(std::localtime(&tt2), "%b %d %Y %H:%M:%S" ) <<endl<<
              "Creation Time: "<< std::put_time(std::localtime(&tt3), "%b %d %Y %H:%M:%S" ) <<endl;

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

    Event ev = Event(eventProp["UID"], eventProp["SUMMARY"],description,location,url,tp_creation, tp_start,tp_end);

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

Event IcalHandler::event_from_ical_component(icalcomponent* comp){
    map<string,string> eventProps;           /* mappa con il nome della proprietà come key e il valore della proprietà come value */
    eventProps = find_properties(comp);      /* estraggo le proprietà dal componente */

    Event ev = event_creator(eventProps);   /* creo il nuovo evento passando le proprietà */

    return ev;

}

Task IcalHandler::task_creator(map<string,string> taskProp) {

    /*
   std::time_t tt1, tt2, tt3;
   tt1 = chrono::system_clock::to_time_t ( tp_start );
   tt2 = chrono::system_clock::to_time_t ( tp_end );
   tt3 = chrono::system_clock::to_time_t ( tp_creation );
   std::cout << "STart Time: "<< std::put_time(std::localtime(&tt1), "%b %d %Y %H:%M:%S" ) <<endl<<
               "End Time: "<< std::put_time(std::localtime(&tt2), "%b %d %Y %H:%M:%S" ) <<endl<<
             "Creation Time: "<< std::put_time(std::localtime(&tt3), "%b %d %Y %H:%M:%S" ) <<endl;

     */

    /*creo l'evento */

    string description;
    bool flagData;                                                   /* flag che segnala la presenza della data */
    std::chrono::time_point<std::chrono::system_clock> tp_due;       /* variabile che salva la data del task *(

    /* converto la priorità da string a int */
    int priority_int = 2;
    //int priority_int = stoi(taskProp["PRIORITY"]);

    /* se non ho la proprietà due date => il task non avrà una data, altrimenti salvo la data e segnalo col flag la presenza della data */
    if (taskProp.find("DUE") != taskProp.end()) {
        tm tm_due = {};
        /* eseguo il parsing della stringa contenente la data */
        strptime(taskProp["DUE"].c_str(), "%Y%m%dT%H%M%SZ", &tm_due);
        /* creo gli oggetti di tipo system_clock */
        tp_due = std::chrono::system_clock::from_time_t(std::mktime(&tm_due));
        flagData = true;


    } else {
        cout<<"non ho trovato la data!"<<endl;
        flagData = false;
    }

    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */
    if (taskProp.find("DESCRIPTION") == taskProp.end()) {
        description = "";
    } else {
        cout<<"non ho trovato description"<<endl;
        description = taskProp["DESCRIPTION"];
    }
    cout<<"flag data vale: "<<flagData<<endl;

    if (flagData == true) {
        /* chiamo il costruttore con data */
        cout<<"uid : "<<taskProp["UID"]<<endl;
        cout<<"summary:" <<taskProp["SUMMARY"]<<endl;
        cout<<"description: "<<description<<endl;
        cout<<"priority int: "<<endl;


        // TEMP Task t(taskProp["UID"], taskProp["SUMMARY"], "ciao", priority_int, tp_due);
        Task t{};
        return t;
    } else {
        cout<<"costruttore senza data"<<endl;
        cout<<"uid : "<<taskProp["UID"]<<endl;
        cout<<"summary:" <<taskProp["SUMMARY"]<<endl;
        cout<<"description: "<<description<<endl;
        cout<<"priority int: "<<priority_int<<endl;
        /* chiamo il costruttore senza data */
        // TEMP Task t(taskProp["UID"], taskProp["SUMMARY"], "ciao", priority_int);
        Task t{};
        cout<<"nome di t: "<<t.getName()<<endl;
        return t;

    }
}

Task IcalHandler::task_from_ical_component(icalcomponent* comp){
    map<string, string> taskProps;           /* mappa con il nome della proprietà come key e il valore della proprietà come value */
    taskProps = find_properties(comp);      /* estraggo le proprietà dal componente */

    for(auto i: taskProps){
        cout<<"key: "<<i.first<<" value: "<<i.second<<endl;
    }

    Task t = task_creator(taskProps);   /* creo il nuovo evento passando le proprietà */
    cout<<"uid di t:"<<t.getUid()<<endl;
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
