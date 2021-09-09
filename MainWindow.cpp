//
// Created by Riccardo Mengoli on 09/09/2021 18:06.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    calendar = new Calendar(this);
    login = new Login(this);


    setMinimumSize(640, 480);
    setCentralWidget(calendar);
}