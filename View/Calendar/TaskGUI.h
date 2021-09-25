//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#ifndef POLICALENDAR_TASKGUI_H
#define POLICALENDAR_TASKGUI_H


#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>

class TaskGUI : public QFrame {
    Q_OBJECT

public:
    TaskGUI(QWidget *parent = nullptr);
};


#endif //POLICALENDAR_TASKGUI_H
