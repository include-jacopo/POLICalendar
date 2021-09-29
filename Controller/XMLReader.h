//
// Created by michele on 9/18/21.
//

#ifndef POLICALENDAR_XMLREADER_H
#define POLICALENDAR_XMLREADER_H

#include <string>
#include <libical/ical.h>
#include <list>
#include <map>

using namespace std;

map<string,icalcomponent*> readXML(string str);
string readCtag(string str);
string readLinkUser (string str);
string readCalendarCollection (string str);
string readUriCalendar (string str);
string readUriTodo (string str);

#endif //POLICALENDAR_XMLREADER_H


