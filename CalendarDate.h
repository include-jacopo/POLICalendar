//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#ifndef POLICALENDAR_CALENDARDATE_H
#define POLICALENDAR_CALENDARDATE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class CalendarDate : public QWidget {
    Q_OBJECT

public:
    explicit CalendarDate(const std::string& date, QWidget *parent = nullptr);
};


#endif //POLICALENDAR_CALENDARDATE_H
