//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#ifndef POLICALENDAR_CALENDAREVENTS_H
#define POLICALENDAR_CALENDAREVENTS_H


#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include "CalendarEvent.h"

class CalendarEvents : public QWidget {
    Q_OBJECT
private:
    bool isLastCol;
    QList<CalendarEvent*> events;

public:
    explicit CalendarEvents(QWidget *widget = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};


#endif //POLICALENDAR_CALENDAREVENTS_H
