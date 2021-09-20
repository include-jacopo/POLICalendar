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
    auto previousColsOnScreen = colsOnScreen;
    colsOnScreen = (this->width() - 50) / 200;
    colsOnScreen = std::clamp(colsOnScreen, 1, NCOLS);

    if (previousColsOnScreen != colsOnScreen) {
        for (int i = 0; i < NCOLS; ++i) {
            auto date = std::get<0>(columns[i]);
            auto events = std::get<1>(columns[i]);
            if (i < colsOnScreen) {
                date->show();
                events->show();
                layout->setColumnStretch(1 + i, 1);
            } else {
                date->hide();
                events->hide();
                layout->setColumnStretch(1 + i, 0);
            }
            // Update property for css right border
            events->setProperty("isLast", i == (colsOnScreen - 1));
        }

        // Necessary to reapply stylesheet after property edit
        setStyleSheet("CalendarEvents[isLast=true] {border-right: 1px solid #086375;}");
    }
}

void CalendarColumns::dateChanged(QDate date) {
    // Generate new columns
    for (int i = 0; i < NCOLS; ++i) {
        std::get<0>(columns[i])->setDate(date.addDays(i));
        std::get<1>(columns[i])->setDate(date.addDays(i));
    }
}

