//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarDate.h"

CalendarDate::CalendarDate(const std::string& date, QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(QString::fromStdString(date)));
}
