//
// Created by Jacopo on 20/09/21.
//

#ifndef POLICALENDAR_ICALHANDLER_H
#define POLICALENDAR_ICALHANDLER_H

#include <libical/ical.h>
#include <map>
#include <string>
#include "../Model/Event.h"
#include "../Model/Task.h"

using namespace std;

class IcalHandler {
public:
    static map<string,string> find_properties(icalcomponent* comp);
    static Event event_creator(map<string,string> eventProp, string etag);
    static Event event_from_ical_component(icalcomponent* comp, string etag);
    static Task task_creator(map<string,string> taskProp, string etag);
    static Task task_from_ical_component(icalcomponent* comp, string etag);

};


#endif //POLICALENDAR_ICALHANDLER_H
