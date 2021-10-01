//
// Created by michele on 9/18/21.
//

#include "XMLReader.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>
#include <libical/ical.h>
#include <list>
#include <map>

using namespace std;

string readCtag(string str) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    auto node = doc.child("d:multistatus").first_child();
    auto node2 = node.child("d:propstat").child("d:prop").child("cs:getctag");

    return node2.text().as_string();
}

map<string,icalcomponent*> readXML(string str) {
    pugi::xml_document doc;
    stringstream ss;
    list<icalcomponent*> lista_eventi;
    map<string,icalcomponent* > mappa_eventi;

    icalcomponent* ic;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    for (auto node: doc.child("d:multistatus").children()) {

        auto nodeEtag = node.child("d:propstat").child("d:prop").child("d:getetag");

        for (auto node2: node.child("d:propstat").child("d:prop").child("cal:calendar-data")) {
            //Inserisco in mappa con chiave etag e value il componente ical
            mappa_eventi.insert({nodeEtag.text().as_string(), icalparser_parse_string(node2.text().as_string())});
        }

    }
    return mappa_eventi;
}

string readLinkUser(string str) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    auto node = doc.child("d:multistatus").first_child();
    auto node2 = node.child("d:propstat").child("d:prop").child("d:current-user-principal").child("d:href");

    return node2.text().as_string();
}

string readCalendarCollection(string str) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    auto node = doc.child("d:multistatus").first_child();
    auto node2 = node.child("d:propstat").child("d:prop").child("cal:calendar-home-set").child("d:href");

    return node2.text().as_string();
}

string readUriCalendar(string str) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    string uri;
    for (auto node: doc.child("d:multistatus").children()) {
        auto temp = node;
        auto node2 = node.child("d:propstat").child("d:prop").child("cal:supported-calendar-component-set");
        for (auto node3: node2.children("cal:comp")) {
            if(string(node3.attribute("name").as_string()) == "VEVENT" ){
               uri = node.child("d:href").text().as_string();
            }
        }
    }
    return uri;
}

string readUriTask(string str) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    string uri;
    for (auto node: doc.child("d:multistatus").children()) {
        auto temp = node;
        auto node2 = node.child("d:propstat").child("d:prop").child("cal:supported-calendar-component-set");
        for (auto node3: node2.children("cal:comp")) {
            if(string(node3.attribute("name").as_string()) == "VTODO" ){
                uri = node.child("d:href").text().as_string();
            }
        }
    }
    return uri;
}

map<string,string> readEtagCalendar (string str, string uri_calendar){
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    string uid, etag;
    map <string, string> event_from_etag;
    for (auto node: doc.child("d:multistatus").children()) {

        //Pesco l'UID dalla Propfind
        auto node2 = node.child("d:href");
        uid = node2.text().as_string();
        //Rimuovo dalla stringa dell'evento l'URI in modo da lasciare solo l'UID
        uid = removeURI(uid, uri_calendar);

        //Pesco l'etag dalla Propfind
        auto node3 = node.child("d:propstat").child("d:prop").child("d:getetag");
        etag = node3.text().as_string();
        //Eseguo la funzione per togliere il '"' all'inizio ed alla fine
        etag = removeQuote(etag);

        event_from_etag.insert({uid, etag});

    }
    return event_from_etag;
}

map<string,string> readEtagTask (string str, string uri_task){
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { // check of the correct loading of the xml
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    string uid, etag;
    map <string, string> task_from_etag;
    for (auto node: doc.child("d:multistatus").children()) {

        //Pesco l'UID dalla Propfind
        auto node2 = node.child("d:href");
        uid = node2.text().as_string();
        //Rimuovo dalla stringa dell'evento l'URI in modo da lasciare solo l'UID
        uid = removeURI(uid, uri_task);

        //Pesco l'etag dalla Propfind
        auto node3 = node.child("d:propstat").child("d:prop").child("d:getetag");
        etag = node3.text().as_string();
        //Eseguo la funzione per togliere il '"' all'inizio ed alla fine
        etag = removeQuote(etag);

        task_from_etag.insert({uid, etag});

    }
    return task_from_etag;
}

string removeQuote (string str){
    //Funzione per rimuovere le virgolette o il "&quot;" dal campo dell'etag
    string remove_quote1 = "&quot;";
    string remove_quote2 = "\"";
    size_t pos;

    while ((pos  = str.find(remove_quote1) )!= std::string::npos) {
        str.erase(pos, remove_quote1.length());
    }
    while ((pos  = str.find(remove_quote2) )!= std::string::npos) {
        str.erase(pos, remove_quote2.length());
    }
    return str;
}

string removeURI (string str, const string& uid){
    size_t pos;
    string ics = ".ics";

    if((pos = str.find(uid)) != string::npos) {
        str.erase(pos, uid.length());
    }
    if((pos = str.find(ics)) != string::npos) {
        str.erase(pos, ics.length());
    }
    return str;
}


