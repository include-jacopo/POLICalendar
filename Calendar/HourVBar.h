//
// Created by Riccardo Mengoli on 10/09/2021 12:48.
//

#ifndef POLICALENDAR_HOURVBAR_H
#define POLICALENDAR_HOURVBAR_H

#include <QWidget>
#include <QFrame>
#include <QList>
#include <QLabel>
#include <QVBoxLayout>

class HourVBar : public QFrame {
    Q_OBJECT

private:
    QList<QLabel*> hours;
    QVBoxLayout *layout;

public:
    explicit HourVBar(QWidget *parent = nullptr);
};


#endif //POLICALENDAR_HOURVBAR_H
