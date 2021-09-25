//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#include "Tasklist.h"

Tasklist::Tasklist(QWidget *parent) : QFrame(parent) {
    // External layout
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,15,0,0);

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
    auto layout2 = new QVBoxLayout();
    layout2->setSpacing(0);
    layout2->setContentsMargins(0, 0, 0, 0);
    w->setLayout(layout2);

    for (int i = 0; i < 14; ++i) {
        auto task = new TaskGUI();
        layout2->addWidget(task);
        task->setProperty("isLast", i==13);
        listOfTasks.push_back(task);
    }

    // Fill empty space at the end
    layout2->insertStretch(-1, 1);
}