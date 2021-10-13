#include "XMLReader.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>
#include <libical/ical.h>
#include <list>
#include <map>

using namespace std;

/**
 * @return a string containing the ctag given the xml file.
 */
string readCtag(const string& str, const string& tag, const string& tag_calserver) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto ctag = tag_calserver+":getctag";

    //lettura del file xml
    auto node = doc.child(multistatus.c_str()).first_child();
    auto node2 = node.child(propstat.c_str()).child(prop.c_str()).child(ctag.c_str());

    return node2.text().as_string();
}

/**
 * Read the events or tasks from the XML file received from the server.
 * @return a map with the etag as key and as a value the ical component.
 */
map<string,icalcomponent*> readXML(const string& str, const string& tag, const string& tag_caldav) {
    pugi::xml_document doc;
    stringstream ss;
    list<icalcomponent*> lista_eventi;
    map<string,icalcomponent* > mappa_eventi;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto calendardata = tag_caldav+":calendar-data";
    auto etag = tag+":getetag";

    //lettura del file xml
    for (auto node: doc.child(multistatus.c_str()).children()) {
        auto nodeEtag = node.child(propstat.c_str()).child(prop.c_str()).child(etag.c_str());
        for (auto node2: node.child(propstat.c_str()).child(prop.c_str()).child(calendardata.c_str())) {
            //Inserisco in mappa con chiave etag e value il componente ical
            mappa_eventi.insert({nodeEtag.text().as_string(), icalparser_parse_string(node2.text().as_string())});
        }
    }
    return mappa_eventi;
}

/**
 * Read the link of the user from the calendar xml file given from the server.
 * @return the link of the user as a string.
 */
string readLinkUser(const string& str, const string& tag) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto currentuserprincipal = tag+":current-user-principal";
    auto href = tag+":href";

    //lettura del file xml
    auto node = doc.child(multistatus.c_str()).first_child();
    auto node2 = node.child(propstat.c_str()).child(prop.c_str()).child(currentuserprincipal.c_str()).child(href.c_str());

    return node2.text().as_string();
}

/**
 * Read the calendar collection of the user.
 * @return the link of the calendar of a user as a string.
 */
string readCalendarCollection(const string& str, const string& tag, const string& tag_caldav) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto calendarhomeset = tag_caldav+":calendar-home-set";
    auto href = tag+":href";

    //lettura del file xml
    auto node = doc.child(multistatus.c_str()).first_child();
    auto node2 = node.child(propstat.c_str()).child(prop.c_str()).child(calendarhomeset.c_str()).child(href.c_str());

    return node2.text().as_string();
}

/**
 * Read the uri of events from the calendar of the user.
 * @return string containing the uri.
 */
string readUriCalendar(const string& str, const string& tag, const string& tag_caldav) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto supportedcalendar = tag_caldav+":supported-calendar-component-set";
    auto href = tag+":href";
    auto comp = tag_caldav+":comp";
    string uri;

    //lettura del file xml
    for (auto node: doc.child(multistatus.c_str()).children()) {
        auto node2 = node.child(propstat.c_str()).child(prop.c_str()).child(supportedcalendar.c_str());
        for (auto node3: node2.children(comp.c_str())) {
            if(string(node3.attribute("name").as_string()) == "VEVENT" ){
                uri = node.child(href.c_str()).text().as_string();
            }
        }
    }
    return uri;
}

/**
 * Read the uri of tasks from the calendar of the user.
 * @return string containing the uri.
 */
string readUriTask(const string& str, const string& tag, const string& tag_caldav) {
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto supportedcalendar = tag_caldav+":supported-calendar-component-set";
    auto href = tag+":href";
    auto comp = tag_caldav+":comp";
    string uri;

    //lettura del file xml
    for (auto node: doc.child(multistatus.c_str()).children()) {
        auto node2 = node.child(propstat.c_str()).child(prop.c_str()).child(supportedcalendar.c_str());
        for (auto node3: node2.children(comp.c_str())) {
            if(string(node3.attribute("name").as_string()) == "VTODO" ){
                uri = node.child(href.c_str()).text().as_string();
            }
        }
    }
    return uri;
}

/**
 * Read the etag for the events from the server response.
 * @return a map with the uid of the event as key and the etag as the value.
 */
map<string,string> readEtagCalendar (const string& str, const string& uri_calendar, const string& tag){
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto getetag = tag+":getetag";
    auto href = tag+":href";
    string uid, etag;
    map <string, string> event_from_etag;

    //lettura del file xml
    for (auto node: doc.child(multistatus.c_str()).children()) {

        //Pesco l'UID dalla Propfind
        auto node2 = node.child(href.c_str());
        uid = node2.text().as_string();
        //Rimuovo dalla stringa dell'evento l'URI in modo da lasciare solo l'UID
        uid = removeURI(uid, uri_calendar);

        //Pesco l'etag dalla Propfind
        auto node3 = node.child(propstat.c_str()).child(prop.c_str()).child(getetag.c_str());
        etag = node3.text().as_string();
        //Eseguo la funzione per togliere il '"' all'inizio ed alla fine
        etag = removeQuote(etag);

        event_from_etag.insert({uid, etag});

    }
    return event_from_etag;
}

/**
 * Read the etag for the tasks from the server response.
 * @return a map with the uid of the task as key and the etag as the value.
 */
map<string,string> readEtagTask (const string& str, const string& uri_task, const string& tag){
    pugi::xml_document doc;
    stringstream ss;

    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) { //controllo il corretto caricamento dell'XML
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }

    //definizione dei campi da leggere
    auto multistatus = tag+":multistatus";
    auto propstat = tag+":propstat";
    auto prop = tag+":prop";
    auto getetag = tag+":getetag";
    auto href = tag+":href";
    string uid, etag;
    map <string, string> task_from_etag;

    //lettura del file xml
    for (auto node: doc.child(multistatus.c_str()).children()) {
        //Pesco l'UID dalla Propfind
        auto node2 = node.child(href.c_str());
        uid = node2.text().as_string();
        //Rimuovo dalla stringa dell'evento l'URI in modo da lasciare solo l'UID
        uid = removeURI(uid, uri_task);
        //Pesco l'etag dalla Propfind
        auto node3 = node.child(propstat.c_str()).child(prop.c_str()).child(getetag.c_str());
        etag = node3.text().as_string();
        //Eseguo la funzione per togliere il '"' all'inizio ed alla fine
        etag = removeQuote(etag);

        task_from_etag.insert({uid, etag});

    }
    return task_from_etag;
}

/**
 * Given a string with quote or "&quot;".
 * @return a string without them.
 */
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

/**
 * Given a string with the uri.
 * @return a string without it and the ics extension.
 */
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


