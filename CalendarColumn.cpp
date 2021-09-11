//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarColumn.h"

CalendarColumn::CalendarColumn(std::string date, bool isLastCol, QWidget *parent) : QWidget(parent) {
    isLast = isLastCol;
    this->date = std::move(date);

    // Inizializza bordi sinistra e destra
    middleLines = new QList<QLineF*>();
    lBorder = new QLineF();
    rBorder = new QLineF();
    for (int i = 0; i < 24+1; ++i) {
        middleLines->push_back(new QLineF());
    }


    // Layout
    layout = new QStackedLayout();
    layout->setStackingMode(QStackedLayout::StackAll);

    //setLayout(layout);

    // Inizializza lista eventi
    events = new QList<CalendarEvent*>();
    events->push_back(new CalendarEvent(this));
    layout->addWidget(events->at(0));
}

void CalendarColumn::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.drawLine(*lBorder);
    if (isLast) {
        painter.drawLine(*rBorder);
    }
    painter.setPen(QColor::fromRgb(205, 205, 205));
    for (auto line : *middleLines){
        painter.drawLine(*line);
    }
}

void CalendarColumn::resizeEvent(QResizeEvent *event) {
    lBorder->setLine(0, 0, 0, this->height());
    if (isLast) {
        rBorder->setLine(this->width()-1, 0, this->width()-1, this->height());
    }
    for (int i = 0; i < 24+1; ++i) {
        if (i != 24) {
            middleLines->at(i)->setLine(5, this->height()/24.0*i, this->width()-5, this->height()/24.0*i);
        } else {
            middleLines->at(i)->setLine(5, this->height()/24.0*i-1, this->width()-5, this->height()/24.0*i-1);
        }
    }
}
