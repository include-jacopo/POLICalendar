//
// Created by Riccardo Mengoli on 09/09/2021 18:06.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    login = new Login(this);

    // Connetti loginSuccessful di login ad azione locale
    connect(login, SIGNAL(loginSuccessful()), this, SLOT(loginSuccessful()));

    setMinimumSize(640, 480);
    setCentralWidget(login);

    //TODO TEMPORARY
    loginSuccessful();
}

MainWindow::~MainWindow() {
    delete calendar;
    delete login;
}

void MainWindow::loginSuccessful() {
    login->close();
    calendar = new Calendar(this);
    setCentralWidget(calendar);
}
