//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#include "Calendar.h"

#define NCOLS 7

Calendar::Calendar(QWidget *parent) : QWidget(parent) {
    // Set background color
    auto palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    // Main grid layout
    auto layoutMain = new QGridLayout(this);

    // Grid layout for columns
    {
        calendarColumns = new QWidget();
        auto layoutCalendarColumns = new QGridLayout(calendarColumns);
        layoutCalendarColumns->setSpacing(0);
        layoutMain->setColumnStretch(0, 1);
        layoutMain->addWidget(calendarColumns, 0, 0, 2, 1);

        // Time bar
        auto hourVBar = new HourVBar();
        hourVBar->setFixedWidth(60);
        layoutCalendarColumns->addWidget(hourVBar, 1, 0);
        layoutCalendarColumns->setRowStretch(1, 1);

        // Calendar daily columns
        for (int i = 0; i < NCOLS; ++i) {
            auto calDate = new CalendarDate(QDate::currentDate().addDays(i));
            layoutCalendarColumns->addWidget(calDate, 0, 1 + i, Qt::AlignCenter);
            auto calEvents = new CalendarEvents(i == NCOLS - 1);
            layoutCalendarColumns->addWidget(calEvents, 1, 1 + i);
            columns.push_back(std::make_tuple(calDate, calEvents));
        }
    }

    // Calendar widget
    layoutMain->addWidget(new QCalendarWidget(), 0, 1);

    // Tasklist widget
    layoutMain->addWidget(new QLabel("TODO Tasklist"), 1, 1); //TODO Create tasklist widget
    layoutMain->setRowStretch(1, 1);
}

void Calendar::resizeEvent(QResizeEvent *event) {
    int nCols = (calendarColumns->width()-60) / 200;
    if (nCols < 1) nCols = 1;
    if (nCols > 7) nCols = 7;
    for (int i = 0; i < columns.size(); ++i) {
        auto date = std::get<0>(columns[i]);
        auto events = std::get<1>(columns[i]);
        if (date != nullptr && events != nullptr) {
            if (i < nCols) {
                date->show();
                events->show();
            } else {
                date->hide();
                events->hide();
            }
        }
    }
}
