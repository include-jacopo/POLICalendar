//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#include "CalendarEvents.h"

CalendarEvents::CalendarEvents(QWidget *widget) : QWidget(widget) {
    // Inizializza lista eventi
    events = QList<CalendarEvent*>();
    events.push_back(new CalendarEvent(14*60, 120, this)); //TODO Get real data
}

void CalendarEvents::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QColor::fromRgb(205, 205, 205));
    for (int i = 0; i < 24+1; ++i) {
        if (i != 24) {
            painter.drawLine(5, (int)(this->height()/24.0*i), this->width()-5, (int)(this->height()/24.0*i));
        } else {
            painter.drawLine(5, (int)(this->height()/24.0*i-1), this->width()-5, (int)(this->height()/24.0*i-1));
        }
    }
}

void CalendarEvents::resizeEvent(QResizeEvent *event) {
    for (auto calendarEvent : events) {
        calendarEvent->setGeometry(5, (int) (this->height() / (24 * 60.0) * calendarEvent->getStartMinute()),
                    this->width() - 10, (int) (this->height() / (24 * 60.0) * calendarEvent->getDurationInMinutes()));
    }
}