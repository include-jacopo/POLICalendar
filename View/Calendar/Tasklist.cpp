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
    layout->setContentsMargins(0, 15, 0, 0);

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
    tasksLayout = new QVBoxLayout();
    tasksLayout->setSpacing(0);
    tasksLayout->setContentsMargins(0, 0, 0, 0);
    w->setLayout(tasksLayout);

    // Filters
    auto filterArea = new QWidget();
    auto filterLayout = new QHBoxLayout(filterArea);
    auto filterCompleted = new QCheckBox("Nascondi task completati");
    filterCompleted->setObjectName("filterCompleted");
    filterLayout->addWidget(filterCompleted);
    filters.insert(filterCompleted, [](TaskGUI *t) { return t->getTask().isCompleted(); });
    layout->addWidget(filterArea);
    connect(filterCompleted, SIGNAL(stateChanged(int)), this, SLOT(updateGUI()));

    // Initialize task list
    auto controller = Controller::getInstance();
    auto mapTasks = controller->getTasks();
    for (const auto &mapTaskIter: mapTasks) {
        auto task = mapTaskIter.second;
        auto taskgui = new TaskGUI(task, this, this);
        taskgui->setProperty("isLast", false);
        tasks.push_back(taskgui);
    }

    // Apply filters and show
    updateGUI();
}

void Tasklist::addTask(const Task &task) {
    auto taskgui = new TaskGUI(task, this, this);
    tasks.push_back(taskgui);

    updateGUI();
}

void Tasklist::removeTask(const Task &task) {
    // Fild TaskGUI in all tasks
    auto it = std::find_if(tasks.constBegin(), tasks.constEnd(), [&task](TaskGUI *t) {
        return t->getTask().getUid() == task.getUid();
    });
    if (it != tasks.constEnd()) {
        qsizetype i = it - tasks.constBegin();
        tasks[i]->deleteLater();
        tasks.remove(i);

        updateGUI();
    }
}

void Tasklist::editTask(const Task &task) {
    auto it = std::find_if(tasks.constBegin(), tasks.constEnd(), [&task](TaskGUI *t) {
        return t->getTask().getUid() == task.getUid();
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

void Tasklist::clearTasks() {
    // Delete all tasks
    for (auto task: tasks) {
        task->deleteLater();
    }
    tasks.clear();
    filteredTasks.clear();
}

void Tasklist::updateTasks() {
    // Clear all the tasks
    clearTasks();

    // Reload the task list
    auto controller = Controller::getInstance();
    auto mapTasks = controller->getTasks();
    for (const auto &mapTaskIter: mapTasks) {
        auto task = mapTaskIter.second;
        auto taskgui = new TaskGUI(task, this, this);
        taskgui->setProperty("isLast", false);
        tasks.push_back(taskgui);
    }

    // Apply filters and show
    updateGUI();
}

void Tasklist::updateGUI() {
    // Hide all tasks
    for (const auto &taskGUI: tasks) {
        taskGUI->hide();
    }

    // Reset last task property
    if (!filteredTasks.isEmpty()) {
        filteredTasks.last()->setProperty("isLast", false);
    }

    // Filter tasks
    QList<TaskGUI *> toBeFilteredTasks;
    filteredTasks = QList(tasks);
    // For each filter
    for (auto it = filters.constBegin(); it != filters.constEnd(); it++) {
        // If the filter is active
        if (it.key()->isChecked()) {
            // For each task still present in filteredTasks
            for (const auto &task: filteredTasks) {
                // Apply the filter and add the element to toBeFilteredTasks if ok
                if (!it.value()(task)) {
                    toBeFilteredTasks.push_back(task);
                }
            }
        } else {
            // If filter not active, all elements are ok
            toBeFilteredTasks = QList(filteredTasks);
        }
        filteredTasks = QList(toBeFilteredTasks);
        toBeFilteredTasks.clear();
    }

    // Remove spacer
    QLayoutItem * child;
    while ((child = tasksLayout->takeAt(0)) != nullptr) {
        tasksLayout->removeItem(child);
    }

    // Move completed task to the end of the list
    std::sort(filteredTasks.begin(), filteredTasks.end(), [](TaskGUI *t1, TaskGUI *t2) {
        auto taskt1 = t1->getTask();
        auto taskt2 = t2->getTask();
        if (taskt1.isCompleted() && !taskt2.isCompleted()) return false;
        return true;
    });

    // Find start index completed tasks
    int iCompleted = 0;
    for (auto t: filteredTasks) {
        if (!t->getTask().isCompleted()) iCompleted++;
    }
    // Order not completed task by date
    std::sort(filteredTasks.begin(), filteredTasks.begin()+iCompleted, [](TaskGUI *t1, TaskGUI *t2) {
        auto taskt1 = t1->getTask();
        auto taskt2 = t2->getTask();
        if (!taskt1.isFlagDate() && taskt2.isFlagDate()) return false;
        if (taskt1.isFlagDate() && taskt2.isFlagDate() &&
            (taskt1.getDueDate() > taskt2.getDueDate())) return false;
        return true;
    });

    // Display taskGUIs
    for (const auto& taskGui: filteredTasks) {
        tasksLayout->addWidget(taskGui);
    }
    tasksLayout->addStretch(1);

    // Show tasks
    for (const auto &taskGUI: filteredTasks) {
        taskGUI->show();
    }

    // Border bottom of list
    if (!filteredTasks.empty()) filteredTasks.last()->setProperty("isLast", true);
    // Necessary to reapply stylesheet after property edit
    setStyleSheet("TaskGUI[isLast=true] {border-bottom: 1px solid #086375;}");
}
