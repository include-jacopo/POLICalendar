//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#ifndef POLICALENDAR_TASKLIST_H
#define POLICALENDAR_TASKLIST_H


#include <QWidget>
#include <QFrame>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include "TaskGUI.h"

class Tasklist : public QFrame {
    Q_OBJECT

private:
    QList<TaskGUI*> listOfTasks;

public:
    explicit Tasklist(QWidget *parent = nullptr);
};


#endif //POLICALENDAR_TASKLIST_H
