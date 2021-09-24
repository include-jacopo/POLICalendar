//
// Created by michele on 9/18/21.
//

#ifndef POLICALENDAR_XMLREADER_H
#define POLICALENDAR_XMLREADER_H

#include <string>
#include <libical/ical.h>
#include <list>

using namespace std;

list<icalcomponent*> readXML(string str);
string readCtag(string str);

#endif //POLICALENDAR_XMLREADER_H


