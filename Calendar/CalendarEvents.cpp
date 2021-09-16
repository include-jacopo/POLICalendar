//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#include "CalendarEvents.h"

CalendarEvents::CalendarEvents(QDate date, QWidget *widget) : QFrame(widget) {
    CalendarEvents::date = date;

    // Inizializza lista eventi
    events = QList<CalendarEvent*>();

    //TODO Get real events
    events.push_back(new CalendarEvent(14 * 60, 120, this));
}

void CalendarEvents::setDate(const QDate &date) {
    CalendarEvents::date = date;
    // TODO Update events
}


void CalendarEvents::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QColor::fromRgb(205, 205, 205));
    for (int i = 0; i < 24 + 1; ++i) {
        if (i != 24) {
            painter.drawLine(5, (int) (this->height() / 24.0 * i), this->width() - 5,
                             (int) (this->height() / 24.0 * i));
        } else {
            painter.drawLine(5, (int) (this->height() / 24.0 * i - 1), this->width() - 5,
                             (int) (this->height() / 24.0 * i - 1));
        }
    }
}

void CalendarEvents::resizeEvent(QResizeEvent *event) {
    for (auto calendarEvent: events) {
        auto sm = calendarEvent->getStartMinute();
        auto dur = calendarEvent->getDurationInMinutes();
        auto tl = QPoint(5, (int) (this->height() / 24.0 * sm / 60));
        auto br = QPoint(this->width() - 5, (int) (this->height() / 24.0 * (sm + dur) / 60));
        calendarEvent->setGeometry(QRect(tl, br));
    }
}
