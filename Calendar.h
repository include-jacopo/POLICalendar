//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#ifndef POLICALENDAR_CALENDAR_H
#define POLICALENDAR_CALENDAR_H

#include <QWidget>
#include <QList>
#include <QHBoxLayout>
#include "CalendarColumn.h"

class Calendar : public QWidget {
    Q_OBJECT
private:
    QList<CalendarColumn*> cols;

public:
    explicit Calendar(QWidget *parent = nullptr);
};


#endif //POLICALENDAR_CALENDAR_H
