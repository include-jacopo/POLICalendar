//
// Created by Riccardo Mengoli on 15/09/2021 16:34.
//

#ifndef POLICALENDAR_CALENDARCOLUMNS_H
#define POLICALENDAR_CALENDARCOLUMNS_H


#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QList>
#include <QDate>
#include "CalendarDate.h"
#include "CalendarEvents.h"
#include "ICalendarGUIEventsHandler.h"

class CalendarColumns : public QFrame, public ICalendarGUIEventsHandler {
    Q_OBJECT

private:
    QList<std::pair<CalendarDate*, CalendarEvents*>> columns;
    QGridLayout *layout;
    QDate firstColDate;
    int colsOnScreen = 0;

    void updateEvents();
    static bool checkIfEventInDay(const Event& e, const QDate& date);

public:
    explicit CalendarColumns(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void dateChanged(QDate date);
    void createEventDialog() override;
    void editEventDialog(const Event &event) override;

protected slots:
    void addEvent(const Event &event) override;
    void editEvent(const Event &event) override;
    void removeEvent(const Event &event) override;
};


#endif //POLICALENDAR_CALENDARCOLUMNS_H
