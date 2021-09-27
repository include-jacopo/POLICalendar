//
// Created by Riccardo Mengoli on 15/09/2021 16:34.
//

#include "HourVBar.h"
#include "../../Controller/Controller.h"
#include "CalendarColumns.h"
#include "DialogEdit.h"

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
        auto calEvents = new CalendarEvents(date, this);
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
    /*
     * This implementation clears all the events and reloads them.
     * More efficient solutions can be implemented in case of dates
     * already displayed on screen.
     */
    for (int i = 0; i < NCOLS; ++i) {
        columns[i].second->clearEvents();
    }

    auto controller = Controller::getInstance();
    auto mapEvents = controller->getEvents();
    for (const auto &mapEventIter: mapEvents) {
        auto event = mapEventIter.second;
        for (int i = 0; i < NCOLS; ++i) {
            auto col = columns[i].second;
            // Add events that happen in the column date
            if (checkIfEventInDay(event, firstColDate.addDays(i))) {
                col->addEvent(event);
            }
        }
    }
}

bool CalendarColumns::checkIfEventInDay(const Event &e, const QDate &date) {
    QDateTime start, end;
    /* Local time start date */
    start = QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(e.getStartTime().time_since_epoch()).count()).toLocalTime();
    /* Local time end date */
    end = QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(e.getEndTime().time_since_epoch()).count()).toLocalTime();
    auto dayStart = date.startOfDay();
    auto dayEnd = date.endOfDay();
    bool c1 = (start >= dayStart && start <= dayEnd);
    bool c2 = (end >= dayStart && end <= dayEnd);
    bool c3 = (start < dayStart && end > dayEnd);
    if (c1 || c2 || c3) {
        return true;
    }
    return false;
}

void CalendarColumns::addEvent(const Event &event) {
    for (int i = 0; i < NCOLS; ++i) {
        auto col = columns[i].second;
        /*
         * Add the event if it takes place during the column date.
         * Note that an event can span two or more columns.
         */
        if (checkIfEventInDay(event, firstColDate.addDays(i))) {
            col->addEvent(event);
        }
    }
}

void CalendarColumns::editEvent(const Event &event) {
    for (int i = 0; i < NCOLS; ++i) {
        auto col = columns[i].second;
        /*
         * Remove the event and then create a new one.
         * This is correct even if an event has its date changed.
         * More efficient solutions can be implemented.
         */
        col->removeEvent(event);
        if (checkIfEventInDay(event, firstColDate.addDays(i))) {
            col->addEvent(event);
        }
    }
}

void CalendarColumns::removeEvent(const Event &event) {
    for (int i = 0; i < NCOLS; ++i) {
        auto col = columns[i].second;
        // Check if the event is in the current column, then delete
        if (checkIfEventInDay(event, firstColDate.addDays(i))) {
            col->removeEvent(event);
        }
    }
}

void CalendarColumns::createEventDialog() {
    auto dialog = new DialogEdit(this);
    connect(dialog, SIGNAL(eventCreated(Event)), this, SLOT(addEvent(Event)));
    dialog->setWindowTitle("Nuovo evento");
    dialog->exec();
}

void CalendarColumns::editEventDialog(const Event &event) {
    auto dialog = new DialogEdit(event, this);
    connect(dialog, SIGNAL(eventEdited(Event)), this, SLOT(editEvent(Event)));
    connect(dialog, SIGNAL(eventDeleted(Event)), this, SLOT(removeEvent(Event)));
    dialog->exec();
}
