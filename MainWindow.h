//
// Created by Riccardo Mengoli on 09/09/2021 18:06.
//

#ifndef POLICALENDAR_MAINWINDOW_H
#define POLICALENDAR_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "Calendar/Calendar.h"
#include "Login/Login.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    Calendar *calendar;
    Login *login;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    [[nodiscard]] QSize sizeHint() const override;

public slots:
    void loginSuccessful();

signals:

};


#endif //POLICALENDAR_MAINWINDOW_H
