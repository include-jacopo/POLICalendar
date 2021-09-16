//
// Created by Riccardo Mengoli on 16/09/2021 11:06.
//

#include "Task.h"

Task::Task(QWidget *parent) : QFrame(parent) {
    // Background color
    auto palette = this->palette();
    palette.setColor(QPalette::Window, QColor::fromRgb(255, 255, 153));
    setPalette(palette);
    setAutoFillBackground(true);

    // Border
    setFrameShape(QFrame::Box);
    setLineWidth(1);

    // Layout
    auto layout = new QVBoxLayout(this);
    auto taskLabel = new QLabel("Task di prova");
    layout->addWidget(taskLabel);
}
