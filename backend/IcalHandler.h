//
// Created by Jacopo on 20/09/21.
//

#ifndef POLICALENDAR_ICALHANDLER_H
#define POLICALENDAR_ICALHANDLER_H

#include <libical/ical.h>
#include <map>
#include <string>
#include "../Model/Event.h"

using namespace std;

class IcalHandler {
public:
    static map<string,string> find_properties(icalcomponent* comp);
    static Event event_creator(map<string,string> eventProp);

};


#endif //POLICALENDAR_ICALHANDLER_H
