//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#ifndef POLICALENDAR_CALENDARDATE_H
#define POLICALENDAR_CALENDARDATE_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QDate>

class CalendarDate : public QWidget {
    Q_OBJECT

public:
    explicit CalendarDate(QDate date, QWidget *parent = nullptr);
};


#endif //POLICALENDAR_CALENDARDATE_H
