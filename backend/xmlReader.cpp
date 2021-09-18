//
// Created by michele on 9/18/21.
//

#include "xmlReader.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> readXML(string str) {

    pugi::xml_document doc;
    stringstream ss;
    vector<string> v;           /* vector of string were we will save the calendar data */
    ss << str;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default | pugi::parse_declaration);

    if (!result) {             /* check of the correct loading of the xml */
        std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    }


    for (auto node: doc.child("d:multistatus").children()) {
        for (auto node2: node.child("d:propstat").child("d:prop").child("cal:calendar-data")) {
            cout << node2.text().as_string() << endl;
                v.push_back(node2.text().as_string());

        }
    }
    return v;
}

