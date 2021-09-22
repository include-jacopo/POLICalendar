//
// Created by Riccardo Mengoli on 09/09/2021 18:06.
//

#ifndef POLICALENDAR_MAINWINDOW_H
#define POLICALENDAR_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "Calendar/Calendar.h"
#include "Login/Login.h"
#include "IView.h"
#include "../Controller/IController.h"

class MainWindow : public QMainWindow, public IView {
    Q_OBJECT

private:
    Calendar *calendar;
    Login *login;
    IController *controller;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    [[nodiscard]] QSize sizeHint() const override;

    void updateEvents() override;
    void setController(IController *c);

public slots:
    void loginSuccessful();
};


#endif //POLICALENDAR_MAINWINDOW_H
