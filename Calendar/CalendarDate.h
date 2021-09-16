//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#ifndef POLICALENDAR_CALENDARDATE_H
#define POLICALENDAR_CALENDARDATE_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QDate>

class CalendarDate : public QFrame {
    Q_OBJECT

private:
    QLabel *labelDate;

public:
    explicit CalendarDate(QDate date, QWidget *parent = nullptr);
    void setDate(const QDate &date);
};


#endif //POLICALENDAR_CALENDARDATE_H
