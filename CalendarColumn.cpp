//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarColumn.h"

CalendarColumn::CalendarColumn(std::string date, bool isLastCol, QWidget *parent) : QWidget(parent) {
    isLast = isLastCol;
    this->date = std::move(date);

    test = new QLabel("Ciao");
    btn = new QPushButton("Hello world!");
    btn->resize(200, 100);

    // Layout verticale
    auto *layout = new QVBoxLayout();
    layout->addWidget(test);
    layout->addWidget(btn);
    setLayout(layout);

    // Inizializza bordi sinistra e destra
    lBorder = new QLineF();
    rBorder = new QLineF();
    for (int i = 0; i < 24+1; ++i) {
        middleLines.push_back(new QLineF());
    }
}

CalendarColumn::~CalendarColumn() {
    delete test;
    delete btn;
    delete lBorder;
    delete rBorder;
    for (auto line : middleLines)
        delete line;
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
    for (auto line : middleLines){
        painter.drawLine(*line);
    }
}

void CalendarColumn::resizeEvent(QResizeEvent *event) {
    lBorder->setLine(0, 0, 0, this->height());
    if (isLast) {
        rBorder->setLine(this->width()-1, 0, this->width()-1, this->height());
    }
    for (int i = 0; i < 24+1; ++i) {
        middleLines[i]->setLine(4, this->height()/24.0*i, this->width()-4, this->height()/24.0*i);
    }
}
