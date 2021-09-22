//
// Created by michele on 9/22/21.
//

#ifndef POLICALENDAR_EVENTSCONTAINER_H
#define POLICALENDAR_EVENTSCONTAINER_H

#include "Event.h"
#include <map>
#include <optional>

using namespace std;

class EventsContainer {
private:
    map<string, Event> Events;
public:
    EventsContainer();
    void addEvent(Event ev);
    optional<Event> findEvent(string uid);
    optional<Event> deleteEvent(string uid);
    void displayEvents();


};


#endif //POLICALENDAR_EVENTSCONTAINER_H
