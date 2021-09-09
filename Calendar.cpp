//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#include "Calendar.h"

#define NCOLS 7

Calendar::Calendar(QWidget *parent) : QWidget(parent) {
    // 7 colonne orizzontali
    auto *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    setMinimumSize(640,480);

    for (int i = 0; i < NCOLS; ++i) {
        cols.push_back(new CalendarColumn("TODO", i==NCOLS-1, this));
        layout->addWidget(cols[i]);
    }

    // Applica il layout
    setLayout(layout);
}
