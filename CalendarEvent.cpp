//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(QWidget *parent) : QWidget(parent) {
    parent->installEventFilter(this);

    QPalette palette(QColor::fromRgb(255, 255, 153, 100));
    setAutoFillBackground(true);
    setPalette(palette);

    layout = new QVBoxLayout(this);
    eventName = new QLabel("Evento di prova");
    layout->addWidget(eventName);
}

bool CalendarEvent::eventFilter(QObject *watched, QEvent *event) {
    float startHour = 14; //TODO
    float hours = 2; //TODO
    auto parent = dynamic_cast<QWidget*>(this->parent());

    if (event->type() == QEvent::Resize) {
        this->setGeometry(5, (float)parent->height()/24*startHour, parent->width()-10, (float)parent->height()/24*hours);
    }
    return false;


    //qDebug() << parent->height() << " --- " << parent->width();

}