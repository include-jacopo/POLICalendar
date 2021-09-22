//
// Created by Riccardo Mengoli on 22/09/2021 18:28.
//

#ifndef POLICALENDAR_ICONTROLLER_H
#define POLICALENDAR_ICONTROLLER_H

using namespace std;

class IController {
public:
    virtual void addEvent(Event ev) = 0;
    virtual optional<Event> findEvent(string uid) = 0;
    virtual optional<Event> deleteEvent(string uid) = 0;
    virtual void displayEvents() = 0;
};

#endif //POLICALENDAR_ICONTROLLER_H
