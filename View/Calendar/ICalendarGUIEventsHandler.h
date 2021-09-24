//
// Created by Riccardo Mengoli on 24/09/2021 17:29.
//

#ifndef POLICALENDAR_ICALENDARGUIEVENTSHANDLER_H
#define POLICALENDAR_ICALENDARGUIEVENTSHANDLER_H

#include <QObject>

class ICalendarGUIEventsHandler : public QObject {
    Q_OBJECT

public:
    virtual ~ICalendarGUIEventsHandler();

public slots:
    virtual void addEvent(const Event &event) = 0;
    virtual void editEvent(const Event &event) = 0;
    virtual void removeEvent(const Event &event) = 0;
};

#endif //POLICALENDAR_ICALENDARGUIEVENTSHANDLER_H
