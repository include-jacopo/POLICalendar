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

class CalendarEvents : public QFrame {
    Q_OBJECT

private:
    QList<CalendarEvent*> events;
    QDate date;

    void updateEvents();

public:
    explicit CalendarEvents(QDate date, QWidget *widget = nullptr);
    void setDate(const QDate &date);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:

};


#endif //POLICALENDAR_CALENDAREVENTS_H
