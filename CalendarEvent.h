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
    QLabel *eventName;
    float startMinute = 14*60; //TODO Read from web
    float durationInMinutes = 120; //TODO Read from web

public:
    explicit CalendarEvent(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif //POLICALENDAR_CALENDAREVENT_H
