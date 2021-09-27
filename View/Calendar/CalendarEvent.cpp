//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <chrono>
#include "CalendarEvent.h"

CalendarEvent::CalendarEvent(const Event &event, ICalendarGUIEventsHandler *handler, QWidget *parent) : QFrame(parent) {
    calEvent = event;
    this->handler = handler;

    /* Local time start date */
    startT = QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(event.getStartTime().time_since_epoch()).count()).toLocalTime();
    /* Local time end date */
    endT = QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(event.getEndTime().time_since_epoch()).count()).toLocalTime();

    QString startTime, endTime;
    startTime = startT.toString("hh:mm");
    endTime = endT.toString("hh:mm");

    // Layout
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(8, 5, 8, 5);

    // Scroll area
    auto scrollarea = new QScrollArea();
    scrollarea->setWidgetResizable(true);
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(scrollarea);

    // Scroll area internal content
    auto w = new QWidget();
    w->setObjectName("ScrollContent");
    scrollarea->setWidget(w);

    auto layout2 = new QVBoxLayout(w);
    layout2->setSpacing(1);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout2->addWidget(new QLabel(QString::fromStdString(event.getName())));
    layout2->addWidget(new QLabel(startTime + " - " + endTime));
    layout2->addWidget(new QLabel(QString::fromStdString(event.getLocation())));
}

QDateTime CalendarEvent::getDateTimeStart() {
    return startT;
}

QDateTime CalendarEvent::getDateTimeEnd() {
    return endT;
}

QString CalendarEvent::getEventUid() {
    return QString::fromStdString(calEvent.getUid());
}

void CalendarEvent::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        handler->editEventDialog(calEvent);
    }
}
