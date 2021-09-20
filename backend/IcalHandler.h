//
// Created by Jacopo on 20/09/21.
//

#ifndef POLICALENDAR_ICALHANDLER_H
#define POLICALENDAR_ICALHANDLER_H

#include <libical/ical.h>

class IcalHandler {
public:
    static void find_properties(icalcomponent* comp);

};


#endif //POLICALENDAR_ICALHANDLER_H
