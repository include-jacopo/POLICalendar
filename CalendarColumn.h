//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#ifndef POLICALENDAR_CALENDARCOLUMN_H
#define POLICALENDAR_CALENDARCOLUMN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QFont>
#include <QStackedLayout>
#include <QLineF>
#include <QPainter>
#include <QList>
#include "CalendarEvent.h"

class CalendarColumn : public QWidget {
    Q_OBJECT
private:
    bool isLast;
    std::string date;

    QStackedLayout *layout;
    QList<CalendarEvent*> events;

public:
    explicit CalendarColumn(std::string date, bool isLastCol = false, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

public slots:

signals:

};


#endif //POLICALENDAR_CALENDARCOLUMN_H
