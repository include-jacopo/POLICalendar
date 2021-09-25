//
// Created by Riccardo Mengoli on 24/09/2021 17:29.
//

#ifndef POLICALENDAR_ICALENDARGUIEVENTSHANDLER_H
#define POLICALENDAR_ICALENDARGUIEVENTSHANDLER_H

class ICalendarGUIEventsHandler {
public:
    virtual void createEventDialog() = 0;
    virtual void editEventDialog(const Event &event) = 0;

protected:
    virtual void addEvent(const Event &event) = 0;
    virtual void editEvent(const Event &event) = 0;
    virtual void removeEvent(const Event &event) = 0;
};

#endif //POLICALENDAR_ICALENDARGUIEVENTSHANDLER_H
