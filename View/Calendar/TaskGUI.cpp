//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#include <QVBoxLayout>
#include <QDateTime>
#include <QLabel>
#include <chrono>
#include "TaskGUI.h"

TaskGUI::TaskGUI(const Task &task, ICalendarGUITaskHandler *handler, QWidget *parent) : QFrame(parent) {
    this->task = task;
    this->handler = handler;

    // Layout
    auto layout = new QVBoxLayout(this);

    // Title label
    auto labelTitle = new QLabel();
    labelTitle->setObjectName("labelTitle");
    layout->addWidget(labelTitle);

    // Due date label
    auto labelDueDate = new QLabel();
    labelDueDate->setObjectName("labelDueDate");
    layout->addWidget(labelDueDate);

    // Fill fields value
    updateFields();
}

QString TaskGUI::getTaskUid() {
    return QString::fromStdString(task.getUid());
}

void TaskGUI::updateTask(const Task &task) {
    this->task = task;
    updateFields();
}

void TaskGUI::updateFields() {
    // Title label
    auto labelTitle = this->findChild<QLabel*>("labelTitle");
    if (task.isCompleted()) {
        labelTitle->setText(QString::fromStdString("<s>" + task.getName() + "</s>"));
    } else {
        labelTitle->setText(QString::fromStdString(task.getName()));
    }

    // Due date label
    auto labelDueDate = this->findChild<QLabel*>("labelDueDate");
    if (task.isFlagDate()) {
        auto dueDateTime = QDateTime::fromSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::seconds>(task.getDueDate().time_since_epoch()).count());
        labelDueDate->setText(dueDateTime.toString("dd/MM/yyyy hh:mm"));
    }
}

void TaskGUI::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        handler->editTaskDialog(task);
    }
}