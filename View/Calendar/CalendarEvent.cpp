//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <chrono>
#include <iomanip>
#include "CalendarEvent.h"
#include "DialogEdit.h"

CalendarEvent::CalendarEvent(const Event& event, QWidget *parent) : QFrame(parent) {
    calEvent = event;

    std::stringstream startTime, endTime;
    auto st = std::chrono::system_clock::to_time_t(event.getStartTime());
    auto end = std::chrono::system_clock::to_time_t(event.getEndTime());
    auto lt = std::localtime(&st);

    // Calculate start minute and create string
    this->startMinute = lt->tm_hour * 60 + lt->tm_min;
    startTime << std::put_time(lt, "%H:%M");

    // Create string end event
    lt = std::localtime(&end);
    endTime << std::put_time(lt, "%H:%M");

    // Calculate event duration
    this->durationInMinutes = std::chrono::duration_cast<std::chrono::minutes>
            (event.getEndTime() - event.getStartTime()).count();

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
    layout2->addWidget(new QLabel(QString::fromStdString(startTime.str() + " - " + endTime.str())));
    layout2->addWidget(new QLabel(QString::fromStdString(event.getLocation())));
}

unsigned int CalendarEvent::getStartMinute() const {
    return startMinute;
}

unsigned int CalendarEvent::getDurationInMinutes() const {
    return durationInMinutes;
}

QString CalendarEvent::getEventUid() {
    return QString::fromStdString(calEvent.getUid());
}

void CalendarEvent::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        auto dialog = new DialogEdit(calEvent, this);
        dialog->exec();
    }
}
