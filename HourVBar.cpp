//
// Created by Riccardo Mengoli on 10/09/2021 12:48.
//

#include "HourVBar.h"

HourVBar::HourVBar(QWidget *parent) : QWidget(parent) {
    hours = new QList<QLabel*>();
    layout = new QVBoxLayout(this);
    auto margins = layout->contentsMargins();
    margins.setTop(0);
    margins.setBottom(0);
    layout->setContentsMargins(margins);
    layout->setAlignment(Qt::AlignRight);
    layout->setSpacing(0);

    for (int i = 0; i < 24; ++i) {
        auto l = new QLabel(QString::number(i));
        auto lMargins = l->contentsMargins();
        lMargins.setTop(0);
        l->setContentsMargins(lMargins);
        l->setAlignment(Qt::AlignTop | Qt::AlignRight);
        hours->push_back(l);
        layout->addWidget(l);
    }
}

HourVBar::~HourVBar() {
    delete hours;
    delete layout;
}
