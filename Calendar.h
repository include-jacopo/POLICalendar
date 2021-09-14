//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#ifndef POLICALENDAR_CALENDAR_H
#define POLICALENDAR_CALENDAR_H

#include <QWidget>
#include <QGridLayout>
#include "HourVBar.h"
#include "CalendarDate.h"
#include "CalendarEvents.h"

class Calendar : public QWidget {
    Q_OBJECT
public:
    explicit Calendar(QWidget *parent = nullptr);
};


#endif //POLICALENDAR_CALENDAR_H
