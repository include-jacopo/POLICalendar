//
// Created by Riccardo Mengoli on 15/09/2021 16:34.
//

#include "CalendarColumns.h"

#define NCOLS 7

CalendarColumns::CalendarColumns(QWidget *parent) : QFrame(parent) {
    layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // Time bar
    auto hourVBar = new HourVBar();
    hourVBar->setFixedWidth(50);
    layout->addWidget(hourVBar, 1, 0);
    layout->setRowStretch(1, 1);

    // Calendar daily columns
    for (int i = 0; i < NCOLS; ++i) {
        auto date = QDate::currentDate().addDays(i);
        auto calDate = new CalendarDate(date);
        auto calEvents = new CalendarEvents(date);
        calDate->layout()->setContentsMargins(0, 0, 0, 10);

        columns.push_back(std::make_tuple(calDate, calEvents));
        layout->addWidget(calDate, 0, 1 + i, Qt::AlignCenter);
        layout->addWidget(calEvents, 1, 1 + i);
    }
}

void CalendarColumns::resizeEvent(QResizeEvent *event) {
    colsOnScreen = (this->width() - 50) / 200;
    colsOnScreen = std::clamp(colsOnScreen, 1, 7);
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
        if (i == colsOnScreen - 1) {
            painter.drawLine(QLine(coord.topRight(), coord.bottomRight()));
        }
    }
}

void CalendarColumns::dateChanged(QDate date) {
    // Generate new columns
    for (int i = 0; i < NCOLS; ++i) {
        std::get<0>(columns[i])->setDate(date.addDays(i));
        std::get<1>(columns[i])->setDate(date.addDays(i));
    }
}
