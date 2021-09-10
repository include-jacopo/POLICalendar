//
// Created by Riccardo Mengoli on 10/09/2021 12:48.
//

#ifndef POLICALENDAR_HOURVBAR_H
#define POLICALENDAR_HOURVBAR_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QVBoxLayout>

class HourVBar : public QWidget {
private:
    QList<QLabel*> *hours;
    QVBoxLayout *layout;


public:
    explicit HourVBar(QWidget *parent = nullptr);
    ~HourVBar() override;

};


#endif //POLICALENDAR_HOURVBAR_H