//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#ifndef POLICALENDAR_TASKGUI_H
#define POLICALENDAR_TASKGUI_H

#include <QWidget>
#include <QFrame>
#include <QMouseEvent>
#include <QString>
#include "../../Model/Task.h"
#include "ICalendarGUITaskHandler.h"

class TaskGUI : public QFrame {
    Q_OBJECT

private:
    Task task;
    ICalendarGUITaskHandler *handler;

    void updateFields();

public:
    TaskGUI(const Task &task, ICalendarGUITaskHandler *handler, QWidget *parent = nullptr);
    QString getTaskUid();
    void updateTask(const Task& task);

protected:
    void mousePressEvent(QMouseEvent *event);
};


#endif //POLICALENDAR_TASKGUI_H
