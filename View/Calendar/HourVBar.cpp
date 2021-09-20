//
// Created by Riccardo Mengoli on 10/09/2021 12:48.
//

#include "HourVBar.h"

HourVBar::HourVBar(QWidget *parent) : QFrame(parent) {
    auto layout = new QVBoxLayout(this);
    auto margins = layout->contentsMargins();
    margins.setTop(0);
    margins.setBottom(0);
    layout->setContentsMargins(margins);
    layout->setSpacing(0);

    for (int i = 0; i < 24; ++i) {
        auto l = new QLabel(QString::number(i) + ":00");
        auto lMargins = l->contentsMargins();
        lMargins.setTop(0);
        l->setContentsMargins(lMargins);
        l->setAlignment(Qt::AlignTop | Qt::AlignRight);
        layout->addWidget(l);
    }
}