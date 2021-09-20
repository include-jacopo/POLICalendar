//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#ifndef POLICALENDAR_CALENDAREVENT_H
#define POLICALENDAR_CALENDAREVENT_H


#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <chrono>
#include <iomanip>
#include "../../Model/Event.h"

class CalendarEvent : public QFrame {
    Q_OBJECT

private:
    unsigned int startMinute;
    unsigned int durationInMinutes;

public:
    explicit CalendarEvent(Event& event, QWidget *parent = nullptr);

    [[nodiscard]] unsigned int getStartMinute() const;
    [[nodiscard]] unsigned int getDurationInMinutes() const;
};


#endif //POLICALENDAR_CALENDAREVENT_H
