//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(unsigned int startMinute, unsigned int durationInMinutes, QWidget *parent) : QFrame(parent) {
    this->startMinute = startMinute;
    this->durationInMinutes = durationInMinutes;

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Evento di prova"));
}

unsigned int CalendarEvent::getStartMinute() const {
    return startMinute;
}

unsigned int CalendarEvent::getDurationInMinutes() const {
    return durationInMinutes;
}
