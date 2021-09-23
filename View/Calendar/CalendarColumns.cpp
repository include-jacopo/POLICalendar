//
// Created by Riccardo Mengoli on 15/09/2021 16:34.
//

#include "HourVBar.h"
#include "../../Controller/Controller.h"
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
    firstColDate = QDate::currentDate();
    for (int i = 0; i < NCOLS; ++i) {
        auto date = firstColDate.addDays(i);
        auto calDate = new CalendarDate(date);
        auto calEvents = new CalendarEvents(date);
        calDate->layout()->setContentsMargins(0, 0, 0, 10);

        columns.push_back(std::make_pair(calDate, calEvents));
        layout->addWidget(calDate, 0, 1 + i, Qt::AlignCenter);
        layout->addWidget(calEvents, 1, 1 + i);
    }

    // Set columns events
    updateEvents();
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
    firstColDate = date;
    // Update dates and events
    for (int i = 0; i < NCOLS; ++i) {
        std::get<0>(columns[i])->setDate(date.addDays(i));
        std::get<1>(columns[i])->setDate(date.addDays(i));
    }
    updateEvents();
}

void CalendarColumns::updateEvents() {
    // Clear current displayed events
    for (int i = 0; i < NCOLS; ++i) {
        columns[i].second->clearEvents();
    }

    auto controller = Controller::getInstance();
    auto mapEvents = controller->getEvents();
    for (const auto& mapEventIter : mapEvents) {
        auto event = mapEventIter.second;
        QDateTime start, end;
        start = QDateTime::fromSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::seconds>(event.getStartTime().time_since_epoch()).count());
        end = QDateTime::fromSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::seconds>(event.getStartTime().time_since_epoch()).count());

        auto firstColDateTime = QDateTime(firstColDate, QTime(0, 0, 0));
        for (int i = 0; i < NCOLS; ++i) {
            auto col = columns[i].second;

            // Add events that happen in the column date
            if (start >= firstColDateTime.addDays(i) && start < firstColDateTime.addDays(i + 1) ||
                (end >= firstColDateTime.addDays(i) && end < firstColDateTime.addDays(i + 1))) {
                col->addEvent(event);
            }
        }
    }
}

