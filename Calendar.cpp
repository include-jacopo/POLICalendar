//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#include "Calendar.h"

Calendar::Calendar(QWidget *parent) : QWidget(parent) {
    // 7 colonne orizzontali
    auto *layout = new QHBoxLayout(this);

    for (int i = 0; i < 7; ++i) {
        cols.push_back(new CalendarColumn(this));
        layout->addWidget(cols[i]);
    }

    // Applica il layout
    setLayout(layout);
}