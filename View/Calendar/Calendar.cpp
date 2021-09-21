//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#include "Calendar.h"

Calendar::Calendar(QWidget *parent) : QFrame(parent) {
    // Stylesheet
    QFile file(QDir::current().filePath("../View/Calendar/Calendar.qss"));
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());

    // Main grid layout
    layout = new QGridLayout(this);
    layout->setSpacing(20);
    setMinimumSize(640, 480);

    // Calendar columns widget
    calendarColumns = new CalendarColumns();
    layout->addWidget(calendarColumns, 0, 0, 2, 1);
    layout->setColumnStretch(0, 1);

    // Calendar widget
    calendarWidget = new QCalendarWidget();
    layout->addWidget(calendarWidget, 0, 1);

    // Tasklist widget
    layout->addWidget(new Tasklist(), 1, 1);
    layout->setRowStretch(1, 1);

    // Connect signal from calendar widget to dateChanged slot
    connect(calendarWidget, &QCalendarWidget::selectionChanged,
            calendarColumns, [this]() {calendarColumns->dateChanged(calendarWidget->selectedDate());});
}

void Calendar::resizeEvent(QResizeEvent *event) {
    auto margin = this->width() / 64;
    margin = std::clamp(margin, 10, 30);
    layout->setContentsMargins(margin, margin, margin, margin);
}

QSize Calendar::sizeHint() const {
    return QSize(1280, 720);
}