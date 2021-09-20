//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#ifndef POLICALENDAR_TASK_H
#define POLICALENDAR_TASK_H


#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>

class Task : public QFrame {
    Q_OBJECT

public:
    Task(QWidget *parent = nullptr);
};


#endif //POLICALENDAR_TASK_H
