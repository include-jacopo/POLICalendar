//
// Created by Riccardo Mengoli on 08/09/2021 17:44.
//

#include <QDir>
#include <QPushButton>
#include "Calendar.h"
#include "DialogEdit.h"

Calendar::Calendar(QWidget *parent) : QFrame(parent) {
    // Stylesheet
    QFile file(QDir::current().filePath("../View/Calendar/Calendar.qss"));
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());

    // Main grid layout
    layout = new QGridLayout(this);
    layout->setSpacing(15);
    setMinimumSize(640, 480);

    // Calendar columns widget
    calendarColumns = new CalendarColumns();
    layout->addWidget(calendarColumns, 0, 0, 3, 1);
    layout->setColumnStretch(0, 1);

    // Calendar widget
    calendarWidget = new QCalendarWidget();
    layout->addWidget(calendarWidget, 0, 1);

    // Tasklist widget
    layout->addWidget(new Tasklist(), 1, 1);
    layout->setRowStretch(1, 1);

    // Add event and task buttons
    auto newButtons = new QFrame();
    auto b_layout = new QHBoxLayout(newButtons);
    b_layout->setContentsMargins(0, 0, 0, 0);
    auto newEvent = new QPushButton("Nuovo evento");
    auto newTask = new QPushButton("Nuovo task");
    b_layout->addWidget(newEvent);
    b_layout->addWidget(newTask);
    layout->addWidget(newButtons, 2, 1, Qt::AlignCenter);

    // Connect signal from calendar widget to dateChanged slot
    connect(calendarWidget, &QCalendarWidget::selectionChanged,
            calendarColumns, [this]() {calendarColumns->dateChanged(calendarWidget->selectedDate());});
    // Connect buttons to dialog
    connect(newEvent, SIGNAL(clicked(bool)), this, SLOT(createNewEvent()));
}

void Calendar::resizeEvent(QResizeEvent *event) {
    auto margin = this->width() / 64;
    margin = std::clamp(margin, 15, 30);
    layout->setContentsMargins(margin, margin, margin, margin);
}

QSize Calendar::sizeHint() const {
    return QSize(1280, 720);
}

void Calendar::createNewEvent() {
    auto dialog = new DialogEdit(this);
    connect(dialog, SIGNAL(eventCreated(Event)), calendarColumns, SLOT(addEvent(Event)));
    dialog->setWindowTitle("Nuovo evento");
    dialog->exec();
}

