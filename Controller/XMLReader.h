#ifndef POLICALENDAR_XMLREADER_H
#define POLICALENDAR_XMLREADER_H

#include <string>
#include <libical/ical.h>
#include <list>
#include <map>
using namespace std;

/**
* General methods.
*/
map<string,icalcomponent*> readXML(const string& str, const string& tag, const string& tag_caldav);
string readCtag(const string& str, const string& tag, const string& tag_calserver);
string readLinkUser (const string& str, const string& tag);
string readCalendarCollection (const string& str, const string& tag, const string& tag_caldav);
string removeQuote (string str);
string removeURI (string str, const string& uid);

/**
* Events methods.
*/
string readUriCalendar (const string& str, const string& tag, const string& tag_caldav);
map<string,string> readEtagCalendar (const string& str, const string& uri_calendar, const string& tag);

/**
* Tasks methods.
*/
string readUriTask (const string& str, const string& tag, const string& tag_caldav);
map<string,string> readEtagTask (const string& str, const string& uri_task, const string& tag);

#endif //POLICALENDAR_XMLREADER_H


