//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#ifndef POLICALENDAR_CALENDAR_H
#define POLICALENDAR_CALENDAR_H

#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QCalendarWidget>
#include <QDir>
#include <QSize>
#include "CalendarColumns.h"
#include "Tasklist.h"

class Calendar : public QFrame {
    Q_OBJECT

private:
    QCalendarWidget *calendarWidget;
    CalendarColumns *calendarColumns;
    QGridLayout *layout;

public:
    explicit Calendar(QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void resizeEvent(QResizeEvent *event) override;
};


#endif //POLICALENDAR_CALENDAR_H
