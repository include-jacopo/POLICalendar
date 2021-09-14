//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#include "CalendarEvents.h"

CalendarEvents::CalendarEvents(bool isLastCol, QWidget *widget) : QWidget(widget) {
    this->isLastCol = isLastCol;

    // Inizializza lista eventi
    events = QList<CalendarEvent*>();
    events.push_back(new CalendarEvent(this));
}

void CalendarEvents::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.drawLine(QLine(1,0,1,this->height()));
    if (isLastCol) {
        painter.drawLine(QLine(this->width()-1, 0, this->width()-1, this->height()));
    }
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