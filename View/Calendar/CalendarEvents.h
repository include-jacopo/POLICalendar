//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#ifndef POLICALENDAR_CALENDAREVENTS_H
#define POLICALENDAR_CALENDAREVENTS_H


#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QDate>
#include "../../Model/Event.h"
#include "CalendarEvent.h"
#include "ICalendarGUIEventsHandler.h"

class CalendarEvents : public QFrame {
    Q_OBJECT

private:
    QList<CalendarEvent*> events;
    QDate date;
    ICalendarGUIEventsHandler *handler;

    enum GeometryEventType {Resize, AddElement, RemoveElement};
    QRect calculateCoords(CalendarEvent *e);
    void setGeometryEvent(CalendarEvent *e, GeometryEventType type);

public:
    explicit CalendarEvents(QDate date, ICalendarGUIEventsHandler *handler, QWidget *parent = nullptr);
    void setDate(const QDate &date);
    void addEvent(const Event &event);
    void removeEvent(const Event &event);
    void clearEvents();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};


#endif //POLICALENDAR_CALENDAREVENTS_H
