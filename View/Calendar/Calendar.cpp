#include <QDir>
#include <QPushButton>
#include <QMessageBox>
#include <QTableView>
#include "Calendar.h"
#include "../../Controller/Controller.h"

Calendar::Calendar(QWidget *parent) : QFrame(parent) {
    // Stylesheet
    QFile file(":/Calendar/Calendar.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();

    // Main grid layout
    layout = new QGridLayout(this);
    layout->setSpacing(15);
    setMinimumSize(640, 480);

    // Calendar columns widget
    calendarColumns = new CalendarColumns();
    layout->addWidget(calendarColumns, 0, 0, 3, 1);
    layout->setColumnStretch(0, 1);
    evHandler = calendarColumns;

    // Calendar widget
    calendarWidget = new QCalendarWidget();
    calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    layout->addWidget(calendarWidget, 0, 1);

    // Tasklist widget
    auto tasklist = new Tasklist();
    layout->addWidget(tasklist, 1, 1);
    layout->setRowStretch(1, 1);
    tHandler = tasklist;

    // Add event and task buttons
    auto newButtons = new QFrame();
    auto b_layout = new QHBoxLayout(newButtons);
    b_layout->setContentsMargins(0, 0, 0, 0);
    auto syncBtn = new QPushButton(QIcon(":/Calendar/sync.svg"), "Sync");
    auto newEvent = new QPushButton("Nuovo evento");
    auto newTask = new QPushButton("Nuovo task");
    b_layout->addWidget(syncBtn);
    b_layout->addSpacing(10);
    b_layout->addWidget(newEvent);
    b_layout->addWidget(newTask);
    layout->addWidget(newButtons, 2, 1, Qt::AlignCenter);

    // Connect signal from calendar widget to dateChanged slot
    connect(calendarWidget, &QCalendarWidget::selectionChanged,
            calendarColumns, [this]() {calendarColumns->dateChanged(calendarWidget->selectedDate());});
    // Connect sync button to controller sync
    connect(syncBtn, SIGNAL(clicked()), this, SLOT(sync()));
    // Connect new event button to dialog
    auto objHandler = dynamic_cast<QObject*>(evHandler);
    connect(newEvent, SIGNAL(clicked()), objHandler, SLOT(createEventDialog()));
    // Connect new task button to dialog
    connect(newTask, SIGNAL(clicked()), tasklist, SLOT(createTaskDialog()));
}

void Calendar::resizeEvent(QResizeEvent *event) {
    auto margin = this->width() / 64;
    margin = std::clamp(margin, 15, 30);
    layout->setContentsMargins(margin, margin, margin, margin);
}

QSize Calendar::sizeHint() const {
    return QSize(1280, 720);
}

void Calendar::sync() {
    auto controller = Controller::getInstance();
    if (controller->sync()) {
        evHandler->updateEvents();
        tHandler->updateTasks();
    } else {
        QMessageBox::warning(this, "Sync", "Sincronizzazione fallita");
    }
}
