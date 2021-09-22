//
// Created by Riccardo Mengoli on 09/09/2021 18:06.
//

#ifndef POLICALENDAR_MAINWINDOW_H
#define POLICALENDAR_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "Calendar/Calendar.h"
#include "Login/Login.h"
#include "../Controller/Controller.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Calendar *calendar;
    Login *login;
    Controller *controller;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setController(Controller *c);

    [[nodiscard]] QSize sizeHint() const override;

public slots:
    void loginSuccessful();

signals:

};


#endif //POLICALENDAR_MAINWINDOW_H
