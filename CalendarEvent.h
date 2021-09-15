//
// Created by Riccardo Mengoli on 10/09/2021 19:26.
//

#ifndef POLICALENDAR_CALENDAREVENT_H
#define POLICALENDAR_CALENDAREVENT_H


#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>

class CalendarEvent : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *layout;
    unsigned int startMinute = 14*60; //TODO Read from web
    unsigned int durationInMinutes = 120; //TODO Read from web

public:
    explicit CalendarEvent(QWidget *parent = nullptr);

    [[nodiscard]] unsigned int getStartMinute() const;
    [[nodiscard]] unsigned int getDurationInMinutes() const;
};


#endif //POLICALENDAR_CALENDAREVENT_H
