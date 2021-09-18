//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#include "Task.h"

Task::Task(QWidget *parent) : QFrame(parent) {
    // Layout
    auto layout = new QVBoxLayout(this);
    auto taskLabel = new QLabel("Task di prova");
    layout->addWidget(taskLabel);
}
