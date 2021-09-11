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

public:
    explicit CalendarEvent(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event);

};


#endif //POLICALENDAR_CALENDAREVENT_H
