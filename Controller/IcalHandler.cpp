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


using namespace std;

/**
     * receives a components and extracts it's properties
     * @return map<string,string> containing the properties and their values
     */
map<string,string> IcalHandler::find_properties(icalcomponent* comp){
    icalproperty* p;

    vector<string> eventProperties; // vector che ospita le varie proprietà dell'evento
    map<string,string> eventProp;

    /* ciclo per ogni proprietà del componente */
    for(p = icalcomponent_get_first_property(comp, ICAL_ANY_PROPERTY); p != 0 ; p = icalcomponent_get_next_property(comp, ICAL_ANY_PROPERTY)){
        /* inserisco nella mappa la coppia proprietà  e corrispondente valore */
        eventProp.insert({icalproperty_get_property_name(p),icalproperty_get_value_as_string(p)});
    }
    return eventProp;
}

/**
     * receives a map with properties and their values and a string the etag of that event and creates an Event
     * @return a Event object
     */
Event IcalHandler::event_creator(map<string,string> eventProp, string etag){

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
    auto tp_start = std::chrono::system_clock::from_time_t(timegm(&tm_start));
    auto tp_end = std::chrono::system_clock::from_time_t(timegm(&tm_end));
    auto tp_creation = std::chrono::system_clock::from_time_t(timegm(&tm_creation));


    stringstream ss_start(eventProp["DTSTART"].c_str());

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
    /* non ho la proprietà url, quindi passo una stringa vuota al costruttore */
    if(eventProp.find("URL")==eventProp.end()){
        url= "";
    }
    else{
        url = eventProp["URL"];
    }
    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */

    Event ev = Event(eventProp["UID"], eventProp["SUMMARY"],description,location,url,etag,tp_creation, tp_start,tp_end);

    return ev;

}

/**
     * wrapper function that receives a component* and the corrisponding etag, cleans the etag and calls a function to create a Event
     * @return a Event object
     */
Event IcalHandler::event_from_ical_component(icalcomponent* comp, string etag){
    map<string,string> eventProps;           /* mappa con il nome della proprietà come key e il valore della proprietà come value */
    eventProps = find_properties(comp);      /* estraggo le proprietà dal componente */

    /* Rimuovo le vigolette o quote dall'etag ottenuto */
    string remove_quote1 = "&quot;";
    string remove_quote2 = "\"";
    size_t pos;

    /* cerco i simboli da rimuovere dalla stringa etag */
    while ((pos  = etag.find(remove_quote1) )!= std::string::npos) {
        etag.erase(pos, remove_quote1.length());
    }
    while ((pos  = etag.find(remove_quote2) )!= std::string::npos) {
        etag.erase(pos, remove_quote2.length());
    }

    Event ev = event_creator(eventProps, etag);   /* creo il nuovo evento passando le proprietà */
    return ev;
}

/**
     * receives a map with properties and their values and a string with the etag of the Task and creates a Task.
     * @return a Task object
     */
Task IcalHandler::task_creator(map<string,string> taskProp, string etag) {
    string description, location;
    bool flagData, flagCompleted;                                                        /* flag che segnala la presenza della data  e della data di completamento */
    int priority;
    std::chrono::time_point<std::chrono::system_clock> tp_due, tp_stamp, tp_compl;       /* variabili che salva la data del task e la data di creazione del task */

    /* converto la priorità da string a int */
    if (taskProp.find("PRIORITY") == taskProp.end()) {
        priority = 0;
    } else {
        /* leggo la priorità dalla mappa e la converto in int */
        priority = stoi(taskProp["PRIORITY"]);
    }

    /* se non ho la proprietà due date => il task non avrà una data, altrimenti salvo la data e segnalo col flag la presenza della data */
    if (taskProp.find("DUE") != taskProp.end()) {
        tm tm_due = {};
        /* eseguo il parsing della stringa contenente la data */
        strptime(taskProp["DUE"].c_str(), "%Y%m%dT%H%M%SZ", &tm_due);
        /* creo gli oggetti di tipo system_clock */
        tp_due = std::chrono::system_clock::from_time_t(timegm(&tm_due));
        flagData = true;

    } else {
        /* non esiste la proprietà DUE quindi setto il flagData a false */
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

    /* se il task è completato allora salvo la data e setto il flag completed a true */
    if (taskProp.find("COMPLETED") != taskProp.end()) {
        tm tm_compl = {};
        /* eseguo il parsing della stringa contenente la data */
        strptime(taskProp["COMPLETED"].c_str(), "%Y%m%dT%H%M%SZ", &tm_compl);
        /* creo gli oggetti di tipo system_clock */
        tp_compl = std::chrono::system_clock::from_time_t(timegm(&tm_compl));
        flagCompleted = true;
    } else {
        flagCompleted = false;
    }

    /* non ho la proprietà description, quindi passo una stringa vuota al costruttore */
    if (taskProp.find("DESCRIPTION") == taskProp.end()) {
        description = "";
    } else {
        description = taskProp["DESCRIPTION"];
    }
    /* non ho la proprietà location, quindi passo una stringa vuota al costruttore */
    if (taskProp.find("LOCATION") == taskProp.end()) {
        location = "";
    } else {
        location = taskProp["LOCATION"];
    }
    /* creo il task */
    Task t(taskProp["UID"],taskProp["SUMMARY"],description,location, move(etag), priority,flagCompleted, flagData, tp_due,tp_stamp,tp_compl);

    return t;
}

/**
     * wrapper function that receives a component* and the corrisponding etag, cleans the etag and calls a function to create a Task
     * @return a Task object
     */
Task IcalHandler::task_from_ical_component(icalcomponent* comp, string etag){
    map<string, string> taskProps;          /* mappa con il nome della proprietà come key e il valore della proprietà come value */
    taskProps = find_properties(comp);      /* estraggo le proprietà dal componente */

    /*Rimuovo le vigolette o quote dall'etag ottenuto */
    string remove_quote1 = "&quot;";
    string remove_quote2 = "\"";
    size_t pos;

    while ((pos  = etag.find(remove_quote1) )!= std::string::npos) {
        etag.erase(pos, remove_quote1.length());
    }
    while ((pos  = etag.find(remove_quote2) )!= std::string::npos) {
        etag.erase(pos, remove_quote2.length());
    }

    Task t = task_creator(taskProps, etag);   /* creo il nuovo evento passando le proprietà */
    
    return t;
}
