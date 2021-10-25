#ifndef POLICALENDAR_CALENDAR_H
#define POLICALENDAR_CALENDAR_H

#include <QWidget>
#include <QFrame>
#include <QCalendarWidget>
#include <QGridLayout>
#include <QSize>
#include "CalendarColumns.h"
#include "Tasklist.h"

class Calendar : public QFrame {
    Q_OBJECT

private:
    QCalendarWidget *calendarWidget;
    CalendarColumns *calendarColumns;
    QGridLayout *layout;
    ICalendarGUIEventsHandler *evHandler;
    ICalendarGUITaskHandler *tHandler;

public:
    explicit Calendar(QWidget *parent = nullptr);
    [[nodiscard]] QSize sizeHint() const override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void sync();
};


#endif //POLICALENDAR_CALENDAR_H
