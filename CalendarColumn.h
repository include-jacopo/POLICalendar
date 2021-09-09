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
#include <QVBoxLayout>
#include <QLine>
#include <QPainter>
#include <QList>

class CalendarColumn : public QWidget {
    Q_OBJECT
private:
    QLabel *test;
    QPushButton *btn;
    QLine *lBorder, *rBorder;
    QList<QLine*> middleLines;

    void swap(CalendarColumn& a, CalendarColumn& b);

public:
    explicit CalendarColumn(QWidget *parent = nullptr);
    CalendarColumn(const CalendarColumn& source);
    CalendarColumn(CalendarColumn&& source);
    CalendarColumn& operator=(CalendarColumn source);
    ~CalendarColumn() override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:

signals:

};


#endif //POLICALENDAR_CALENDARCOLUMN_H
