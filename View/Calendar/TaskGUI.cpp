//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#include <QVBoxLayout>
#include <QDateTime>
#include <QString>
#include <QLabel>
#include <chrono>
#include "TaskGUI.h"

TaskGUI::TaskGUI(const Task &task, ICalendarGUITaskHandler *handler, QWidget *parent) : QFrame(parent) {
    this->task = task;
    this->handler = handler;

    // Layout
    auto layout = new QVBoxLayout(this);

    // Title label
    QString titleQString;
    if (task.isCompleted()) {
        titleQString = QString::fromStdString("<s>" + task.getName() + "</s>");
    } else {
        titleQString = QString::fromStdString(task.getName());
    }
    layout->addWidget(new QLabel(titleQString));

    // Due date label
    if (task.isFlagDate()) {
        auto dueDate = QDateTime::fromSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::seconds>(task.getDate().time_since_epoch()).count());
        layout->addWidget(new QLabel(dueDate.toString("hh:mm")));
    }
}


void TaskGUI::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        handler->editTaskDialog(task);
    }
}