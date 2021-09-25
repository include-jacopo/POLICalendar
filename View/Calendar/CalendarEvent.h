//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#ifndef POLICALENDAR_CALENDAREVENT_H
#define POLICALENDAR_CALENDAREVENT_H


#include <QWidget>
#include <QFrame>
#include <QDateTime>
#include <QString>
#include <QMouseEvent>
#include "../../Model/Event.h"
#include "ICalendarGUIEventsHandler.h"

class CalendarEvent : public QFrame {
    Q_OBJECT

private:
    Event calEvent;
    ICalendarGUIEventsHandler *handler;
    QDateTime startT, endT;

public:
    explicit CalendarEvent(const Event& event, ICalendarGUIEventsHandler *handler, QWidget *parent = nullptr);

    QDateTime getDateTimeStart();
    QDateTime getDateTimeEnd();
    QString getEventUid();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};


#endif //POLICALENDAR_CALENDAREVENT_H
