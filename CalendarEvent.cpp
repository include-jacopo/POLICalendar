//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(QWidget *parent) : QWidget(parent) {
    QPalette palette(QColor::fromRgb(255, 255, 153, 150));
    setAutoFillBackground(true);
    setPalette(palette);

    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Evento di prova"));
}

unsigned int CalendarEvent::getStartMinute() const {
    return startMinute;
}

unsigned int CalendarEvent::getDurationInMinutes() const {
    return durationInMinutes;
}
