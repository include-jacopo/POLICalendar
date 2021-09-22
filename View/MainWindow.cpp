//
// Created by Riccardo Mengoli on 09/09/2021 18:06.
//

#include <QDir>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Stylesheet
    QFile file(QDir::current().filePath("../View/AppStyle.qss"));
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());

    // Login component
    login = new Login(this);

    // Connetti loginSuccessful di login ad azione locale
    connect(login, SIGNAL(loginSuccessful()), this, SLOT(loginSuccessful()));

    resize(login->sizeHint());
    setCentralWidget(login);

    //TODO TEMPORARY
    loginSuccessful();
}

void MainWindow::loginSuccessful() {
    login->close();
    calendar = new Calendar(this);
    resize(calendar->sizeHint());
    setCentralWidget(calendar);
}

QSize MainWindow::sizeHint() const {
    return QSize(1280, 720);
}

void MainWindow::updateEvents() {
    // TODO
}

void MainWindow::setController(IController *c) {
    controller = c;
}
