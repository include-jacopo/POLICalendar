//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(QWidget *parent) : QWidget(parent) {
    parent->installEventFilter(this);

    QPalette palette(QColor::fromRgb(255, 255, 153, 150));
    setAutoFillBackground(true);
    setPalette(palette);

    layout = new QVBoxLayout(this);
    eventName = new QLabel("Evento di prova");
    layout->addWidget(eventName);
}

bool CalendarEvent::eventFilter(QObject *watched, QEvent *event) {
    auto parent = dynamic_cast<QWidget*>(this->parent());

    if (event->type() == QEvent::Resize) {
        this->setGeometry(5, (int)((float)parent->height()/(24*60)*startMinute),
                          parent->width()-10, (int)((float)parent->height()/(24*60)*durationInMinutes));
    }
    return false;
}