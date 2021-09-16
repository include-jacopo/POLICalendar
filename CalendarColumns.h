//
// Created by Riccardo Mengoli on 15/09/2021 16:34.
//

#ifndef POLICALENDAR_CALENDARCOLUMNS_H
#define POLICALENDAR_CALENDARCOLUMNS_H


#include <QWidget>
#include <QGridLayout>
#include <QList>
#include <QPainter>
#include "HourVBar.h"
#include "CalendarDate.h"
#include "CalendarEvents.h"

class CalendarColumns : public QWidget {
private:
    QList<std::tuple<CalendarDate*, CalendarEvents*>> columns;
    QGridLayout *layout;

    int colsOnScreen = 0;

public:
    explicit CalendarColumns(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void dateChanged(QDate date);
};


#endif //POLICALENDAR_CALENDARCOLUMNS_H
