//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#include <QScrollArea>
#include <QVBoxLayout>
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
    auto layout2 = new QVBoxLayout();
    layout2->setSpacing(0);
    layout2->setContentsMargins(0, 0, 0, 0);
    w->setLayout(layout2);

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
        addTask(task);
    }

    // Fill empty space at the end
    layout2->insertStretch(-1, 1);
}

void Tasklist::addTask(const Task &task) {
    auto taskgui = new TaskGUI(task, this, this);
    taskgui->setProperty("isLast", true);
    taskgui->show();
    tasks.last()->setProperty("isLast", false);
    tasks.push_back(taskgui);
}

void Tasklist::removeTask(const Task &task) {
    //TODO
}

void Tasklist::editTask(const Task &task) {
    //TODO
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
