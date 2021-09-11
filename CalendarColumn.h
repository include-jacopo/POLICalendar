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
#include <QLineF>
#include <QPainter>
#include <QList>

class CalendarColumn : public QWidget {
    Q_OBJECT
private:
    bool isLast;
    std::string date;

    QLabel *test;
    QPushButton *btn;
    QLineF *lBorder, *rBorder;
    QList<QLineF*> middleLines;

public:
    explicit CalendarColumn(std::string date, bool isLastCol = false, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:

signals:

};


#endif //POLICALENDAR_CALENDARCOLUMN_H
