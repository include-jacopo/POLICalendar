//
// Created by Riccardo Mengoli on 15/09/2021 16:34.
//

#include "CalendarColumns.h"

#define NCOLS 7

CalendarColumns::CalendarColumns(QWidget *parent) : QWidget(parent) {
    auto layout = new QGridLayout(this);
    layout->setSpacing(0);

    // Time bar
    auto hourVBar = new HourVBar();
    hourVBar->setFixedWidth(60);
    layout->addWidget(hourVBar, 1, 0);
    layout->setRowStretch(1, 1);

    // Calendar daily columns
    for (int i = 0; i < NCOLS; ++i) {
        auto calDate = new CalendarDate(QDate::currentDate().addDays(i));
        layout->addWidget(calDate, 0, 1 + i, Qt::AlignCenter);
        auto calEvents = new CalendarEvents(i == NCOLS - 1);
        layout->addWidget(calEvents, 1, 1 + i);
        columns.push_back(std::make_tuple(calDate, calEvents));
    }
}

void CalendarColumns::resizeEvent(QResizeEvent *event) {
    colsOnScreen = (this->width() - 60) / 200;
    if (colsOnScreen < 1) colsOnScreen = 1;
    if (colsOnScreen > 7) colsOnScreen = 7;
    for (int i = 0; i < columns.size(); ++i) {
        auto date = std::get<0>(columns[i]);
        auto events = std::get<1>(columns[i]);
        if (date != nullptr && events != nullptr) {
            if (i < colsOnScreen) {
                date->show();
                events->show();
            } else {
                date->hide();
                events->hide();
            }
        }
    }
}

void CalendarColumns::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::blue);
    for (int i = 0; i < colsOnScreen; ++i) {
        auto coord = std::get<1>(columns[i])->geometry();
        painter.drawLine(QLine(coord.topLeft(), coord.bottomLeft()));
        if (i == colsOnScreen-1) {
            painter.drawLine(QLine(coord.topRight(), coord.bottomRight()));
        }
    }
}
