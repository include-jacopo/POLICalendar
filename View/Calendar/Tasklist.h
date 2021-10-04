//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#ifndef POLICALENDAR_TASKLIST_H
#define POLICALENDAR_TASKLIST_H


#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QCheckBox>
#include "TaskGUI.h"
#include "ICalendarGUITaskHandler.h"
#include "../../Model/Task.h"

class Tasklist : public QFrame, public ICalendarGUITaskHandler {
    Q_OBJECT

private:
    QList<TaskGUI*> tasks, filteredTasks;
    QVBoxLayout *tasksLayout;
    QHash<QCheckBox*, function<bool(TaskGUI*)>> filters;

    void clearTasks();

public:
    explicit Tasklist(QWidget *parent = nullptr);

public slots:
    void createTaskDialog() override;
    void editTaskDialog(const Task &task) override;
    void updateTasks() override;

protected slots:
    void addTask(const Task &task) override;
    void removeTask(const Task &tasks) override;
    void editTask(const Task &task) override;

private slots:
    void updateGUI();
};


#endif //POLICALENDAR_TASKLIST_H
