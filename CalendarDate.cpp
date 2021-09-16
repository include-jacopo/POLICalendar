//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarDate.h"

CalendarDate::CalendarDate(QDate date, QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    labelDate = new QLabel();
    setDate(date);
    layout->addWidget(labelDate);
}

void CalendarDate::setDate(const QDate &date) {
    labelDate->setText(date.toString(QString("dd/MM/yyyy")));
}
