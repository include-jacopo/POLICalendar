//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#include <QVBoxLayout>
#include <QDateTime>
#include <QLabel>
#include <chrono>
#include <QFile>
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

    // Set property
    setProperty("isCompleted", task.isCompleted());

    // Fill fields value
    updateFields();
}

Task TaskGUI::getTask() {
    return task;
}

void TaskGUI::updateTask(const Task &task) {
    this->task = task;
    updateFields();
}

void TaskGUI::updateFields() {
    // Title label
    auto labelTitle = this->findChild<QLabel *>("labelTitle");
    if (task.isCompleted()) {
        labelTitle->setText(QString::fromStdString("<s>" + task.getName() + "</s>"));
    } else {
        labelTitle->setText(QString::fromStdString(task.getName()));
    }

    // Due date label
    auto labelDueDate = this->findChild<QLabel *>("labelDueDate");
    QDateTime dueDateTime;
    if (task.isFlagDate()) {
        dueDateTime = QDateTime::fromSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::seconds>(task.getDueDate().time_since_epoch()).count());
        labelDueDate->setText(dueDateTime.toString("dd/MM/yyyy hh:mm"));
    } else {
        labelDueDate->setText("");
    }

    // Is expired background color
    setProperty("isCompleted", task.isCompleted());
    setProperty("isExpired", (!task.isCompleted()) && task.isFlagDate() && (dueDateTime < QDateTime::currentDateTime()));

    // Reapply stylesheet
    QFile file(":/Calendar/TaskGUI.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();
}

void TaskGUI::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        handler->editTaskDialog(task);
    }
}