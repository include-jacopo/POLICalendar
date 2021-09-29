//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#ifndef POLICALENDAR_TASKLIST_H
#define POLICALENDAR_TASKLIST_H


#include <QWidget>
#include <QFrame>
#include "TaskGUI.h"
#include "ICalendarGUITaskHandler.h"
#include "../../Model/Task.h"

class Tasklist : public QFrame, public ICalendarGUITaskHandler {
    Q_OBJECT

private:
    QList<TaskGUI*> tasks;

public:
    explicit Tasklist(QWidget *parent = nullptr);

public slots:
    void createTaskDialog() override;
    void editTaskDialog(const Task &task) override;

protected slots:
    void addTask(const Task &task) override;
    void removeTask(const Task &task) override;
    void editTask(const Task &task) override;
};


#endif //POLICALENDAR_TASKLIST_H
