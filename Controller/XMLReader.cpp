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

            mappa_eventi.insert({nodeEtag.text().as_string(), icalparser_parse_string(node2.text().as_string())});

        }

    }
    //return lista_eventi;
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

string readUriTodo(string str) {
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

