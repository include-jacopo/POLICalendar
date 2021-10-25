#include <QDir>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Stylesheet
    QFile file(":/AppStyle.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());
    file.close();

    // Login component
    login = new Login(this);

    // Connetti loginSuccessful di login ad azione locale
    connect(login, SIGNAL(loginSuccessful()), this, SLOT(loginSuccessful()));

    resize(login->sizeHint());
    setCentralWidget(login);
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
