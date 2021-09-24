//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#include "CalendarEvents.h"

CalendarEvents::CalendarEvents(QDate date, ICalendarGUIEventsHandler *handler, QWidget *widget) : QFrame(widget) {
    CalendarEvents::date = date;
    this->handler = handler;
}

void CalendarEvents::setDate(const QDate &date) {
    CalendarEvents::date = date;
}

void CalendarEvents::addEvent(const Event &event) {
    auto calEv = new CalendarEvent(event, handler, this);
    setGeometryEvent(calEv);
    calEv->show();
    events.push_back(calEv);
}

bool CalendarEvents::removeEvent(const Event &event) {
    qsizetype i = events.indexOf(event);
    if (i != -1) {
        delete events[i];
        events.remove(i);
    }
}

void CalendarEvents::clearEvents() {
    for (auto calEv: events) {
        delete calEv;
    }
    events.clear();
}


void CalendarEvents::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
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
        setGeometryEvent(calendarEvent);
    }
}

void CalendarEvents::setGeometryEvent(CalendarEvent *e) {
    auto sm = e->getStartMinute();
    auto dur = e->getDurationInMinutes();
    auto tl = QPoint(5, (int) (this->height() / 24.0 * sm / 60));
    auto br = QPoint(this->width() - 5, (int) (this->height() / 24.0 * (sm + dur) / 60));
    e->setGeometry(QRect(tl, br));
}