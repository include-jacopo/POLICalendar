//
// Created by Riccardo Mengoli on 16/09/2021 11:03.
//

#include "Tasklist.h"

Tasklist::Tasklist(QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    auto title = new QLabel("Tasklist");
    title->setFont(QFont("Helvetica", 14));
    layout->addWidget(title);

    for (int i = 0; i < 3; ++i) {
        auto task = new Task();
        layout->addWidget(task);
        listOfTasks.push_back(task);
    }

    // Remove internal spacing
    layout->setContentsMargins(0, 0, 0, 0);

    // Fill empty space at the end
    layout->insertStretch(-1, 1);
}