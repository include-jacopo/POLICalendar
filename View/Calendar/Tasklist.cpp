//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#include "Tasklist.h"

Tasklist::Tasklist(QWidget *parent) : QFrame(parent) {
    auto layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 15, 0, 0);

    auto title = new QLabel("Tasklist");
    layout->addWidget(title, 0, Qt::AlignCenter);

    for (int i = 0; i < 3; ++i) {
        auto task = new Task();
        layout->addWidget(task);
        task->setProperty("isLast", i==2);
        listOfTasks.push_back(task);
    }

    // Fill empty space at the end
    layout->insertStretch(-1, 1);
}