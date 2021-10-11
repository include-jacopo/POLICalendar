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

map<string,icalcomponent*> readXML(string str, string tag, string tag_caldav);
string readCtag(string str, string tag, string tag_calserver);
string readLinkUser (string str, string tag);
string readCalendarCollection (string str, string tag, string tag_caldav);
string readUriCalendar (string str, string tag, string tag_caldav);
string readUriTask (string str, string tag, string tag_caldav);
map<string,string> readEtagCalendar (string str, string uri_calendar, string tag);
map<string,string> readEtagTask (string str, string uri_task, string tag);
string removeQuote (string str);
string removeURI (string str, const string& uid);

#endif //POLICALENDAR_XMLREADER_H


