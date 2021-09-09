//
// Created by Riccardo Mengoli on 08/09/2021 17:34.
//

#include "CalendarColumn.h"

CalendarColumn::CalendarColumn(QWidget *parent) : QWidget(parent) {
    test = new QLabel("Ciao");
    btn = new QPushButton("Hello world!");
    btn->resize(200, 100);

    // Layout verticale
    auto *layout = new QVBoxLayout();
    layout->addWidget(test);
    layout->addWidget(btn);
    setLayout(layout);

    // Inizializza bordi sinistra e destra
    lBorder = new QLine();
    rBorder = new QLine();
    for (int i = 0; i < 24+1; ++i) {
        middleLines.push_back(new QLine());
    }
}

CalendarColumn::CalendarColumn(const CalendarColumn& source) {
    test = new QLabel(source.test);
    btn = new QPushButton(source.btn);
}

CalendarColumn::CalendarColumn(CalendarColumn&& source) {
    test = nullptr;
    btn = nullptr;
    swap(*this, source);
}

CalendarColumn& CalendarColumn::operator=(CalendarColumn source) {
    swap(*this, source);
    return *this;
}

void CalendarColumn::swap(CalendarColumn& a, CalendarColumn& b) {
    std::swap(a.test, b.test);
    std::swap(a.btn, b.btn);
}

CalendarColumn::~CalendarColumn() {
    delete test;
    delete btn;
}

void CalendarColumn::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.drawLine(*lBorder);
    painter.drawLine(*rBorder);
    painter.setPen(QColor::fromRgb(205, 205, 205));
    for (auto line : middleLines){
        painter.drawLine(*line);
    }
}

void CalendarColumn::resizeEvent(QResizeEvent *event) {
    lBorder->setLine(0, 0, 0, this->height());
    rBorder->setLine(this->width(), 0, this->width(), this->height());
    for (int i = 0; i < 24+1; ++i) {
        middleLines[i]->setLine(2, this->height()/24*i, this->width()-2, this->height()/24*i);
    }
}
