//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#include "CalendarEvents.h"

using namespace std::literals::chrono_literals; //TODO TEMP

CalendarEvents::CalendarEvents(QDate date, QWidget *widget) : QFrame(widget) {
    CalendarEvents::date = date;

    // Inizializza lista eventi
    events = QList<CalendarEvent*>();
    updateEvents();
}

void CalendarEvents::setDate(const QDate &date) {
    CalendarEvents::date = date;
    updateEvents();
}

void CalendarEvents::updateEvents() {
    events.clear();
    //TODO Get real events
    auto test = new Event("1000202", "Evento di prova", "Descrizione di prova", "Mare",
                          std::chrono::system_clock::now(), std::chrono::system_clock::now(), std::chrono::system_clock::now() + 3h);
    events.push_back(new CalendarEvent(*test, this));
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
