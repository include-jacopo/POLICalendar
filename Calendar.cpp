//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#include "Calendar.h"

Calendar::Calendar(QWidget *parent) : QWidget(parent) {
    // Set background color
    auto palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    // Main grid layout
    auto layoutMain = new QGridLayout(this);

    // Calendar columns widget
    calendarColumns = new CalendarColumns();
    layoutMain->addWidget(calendarColumns, 0, 0, 2, 1);
    layoutMain->setColumnStretch(0, 1);

    // Calendar widget
    calendarWidget = new QCalendarWidget();
    layoutMain->addWidget(calendarWidget, 0, 1);

    // Tasklist widget
    layoutMain->addWidget(new QLabel("TODO Tasklist"), 1, 1); //TODO Create tasklist widget
    layoutMain->setRowStretch(1, 1);

    // Connect signal from calendar widget to dateChanged slot
    connect(calendarWidget, &QCalendarWidget::selectionChanged,
            calendarColumns, [this]() {calendarColumns->dateChanged(calendarWidget->selectedDate());});
}