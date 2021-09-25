//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#include "TaskGUI.h"

TaskGUI::TaskGUI(QWidget *parent) : QFrame(parent) {
    // Layout
    auto layout = new QVBoxLayout(this);
    auto taskLabel = new QLabel("Task di prova");
    layout->addWidget(taskLabel);
}
