//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarColumn.h"

CalendarColumn::CalendarColumn(std::string date, bool isLastCol, QWidget *parent) : QWidget(parent) {
    isLast = isLastCol;
    this->date = std::move(date);

    // Layout
    layout = new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackAll);

    //setLayout(layout);

    // Inizializza lista eventi
    events = QList<CalendarEvent*>();
    events.push_back(new CalendarEvent(this));
    layout->addWidget(events.at(0));
}

void CalendarColumn::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.drawLine(QLine(0,0,0,this->height()));
    if (isLast) {
        painter.drawLine(QLine(this->width()-1, 0, this->width()-1, this->height()));
    }
    painter.setPen(QColor::fromRgb(205, 205, 205));
    for (int i = 0; i < 24+1; ++i) {
        if (i != 24) {
            painter.drawLine(5, this->height()/24.0*i, this->width()-5, this->height()/24.0*i);
        } else {
            painter.drawLine(5, this->height()/24.0*i-1, this->width()-5, this->height()/24.0*i-1);
        }
    }
}