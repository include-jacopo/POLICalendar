//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarDate.h"

CalendarDate::CalendarDate(QDate date, QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(date.toString(QString("dd/MM/yyyy"))));
}
