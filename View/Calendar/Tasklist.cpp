//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#include <QScrollArea>
#include <QLabel>
#include "Tasklist.h"
#include "../../Controller/Controller.h"
#include "DialogTask.h"

Tasklist::Tasklist(QWidget *parent) : QFrame(parent) {
    // External layout
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,15,0,0);

    // Title
    auto title = new QLabel("Tasklist");
    layout->addWidget(title, 0, Qt::AlignCenter);

    // Scroll area
    auto scrollarea = new QScrollArea();
    scrollarea->setWidgetResizable(true);
    layout->addWidget(scrollarea);

    // Scroll area internal content
    auto w = new QWidget();
    w->setObjectName("ScrollContent");
    scrollarea->setWidget(w);

    // Internal layout
    auto tasksLayout = new QVBoxLayout();
    tasksLayout->setSpacing(0);
    tasksLayout->setContentsMargins(0, 0, 0, 0);
    w->setLayout(tasksLayout);

    /*for (int i = 0; i < 14; ++i) {
        auto task = new TaskGUI(task, this, this);
        layout2->addWidget(task);
        task->setProperty("isLast", i==13);
        tasks.push_back(task);

    }
    */

    // Initialize task list
    auto controller = Controller::getInstance();
    auto mapTasks = controller->getTasks();
    for (const auto &mapTaskIter: mapTasks) {
        auto task = mapTaskIter.second;
        auto taskgui = new TaskGUI(task, this, this);
        taskgui->setProperty("isLast", false);
        taskgui->show();
        tasks.push_back(taskgui);
        tasksLayout->addWidget(taskgui);
    }
    tasks.last()->setProperty("isLast", true);

    // Fill empty space at the end
    tasksLayout->insertStretch(-1, 1);
}

void Tasklist::addTask(const Task &task) {
    auto taskgui = new TaskGUI(task, this, this);
    taskgui->setProperty("isLast", true);
    taskgui->show();
    tasks.last()->setProperty("isLast", false);
    tasks.push_back(taskgui);
    tasksLayout->addWidget(taskgui);
}

void Tasklist::removeTask(const Task &task) {
    auto it = std::find_if(tasks.constBegin(), tasks.constEnd(), [&task](TaskGUI *t) {
        return t->getTaskUid() == QString::fromStdString(task.getUid());
    });
    if (it != tasks.constEnd()) {
        qsizetype i = it - tasks.constBegin();
        tasksLayout->removeWidget(tasks[i]);
        delete tasks[i];
        tasks.remove(i);
    }
}

void Tasklist::editTask(const Task &task) {
    auto it = std::find_if(tasks.constBegin(), tasks.constEnd(), [&task](TaskGUI *t) {
        return t->getTaskUid() == QString::fromStdString(task.getUid());
    });
    if (it != tasks.constEnd()) {
        qsizetype i = it - tasks.constBegin();
        tasks[i]->updateTask(task);
    }
}

void Tasklist::createTaskDialog() {
    auto dialog = new DialogTask(this);
    connect(dialog, SIGNAL(taskCreated(Task)), this, SLOT(addTask(Task)));
    dialog->setWindowTitle("Nuovo task");
    dialog->exec();
}

void Tasklist::editTaskDialog(const Task &task) {
    auto dialog = new DialogTask(task, this);
    connect(dialog, SIGNAL(taskEdited(Task)), this, SLOT(editTask(Task)));
    connect(dialog, SIGNAL(taskDeleted(Task)), this, SLOT(removeTask(Task)));
    dialog->exec();
}
