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
string readUriTask (string str);
map<string,string> readEtagCalendar (string str, string uri_calendar);
map<string,string> readEtagTask (string str, string uri_task);
string removeQuote (string str);
string removeURI (string str, const string& uid);

#endif //POLICALENDAR_XMLREADER_H


