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

    // Grid 2 righe - NCOLS+1 colonne
    auto layout = new QGridLayout(this);
    layout->setSpacing(0);
    setMinimumSize(640,480);

    // Time bar
    auto hourVBar = new HourVBar();
    hourVBar->setMaximumWidth(60);
    layout->addWidget(hourVBar, 1, 0);
    layout->setRowStretch(1, 1);

    // Calendar daily columns
    for (int i = 0; i < NCOLS; ++i) {
        layout->addWidget(new CalendarDate(QDate::currentDate().addDays(i)), 0, 1+i, Qt::AlignCenter);
        layout->addWidget(new CalendarEvents(i==NCOLS-1), 1, 1+i);
    }
}
