#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../Calendar.h"
#include <QMainWindow>
#include <QMessageBox> //per mostrare l'errore di login

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username == "jacopo" && password == "admin"){

        //QMainWindow mainW = QMainWindow();
        //Calendar raggruppa le CalendarColumn
        auto calendar = new Calendar(this);
        //mainW.setBaseSize(640, 480);

        //mainW.setCentralWidget(calendar);
        //mainW.show();
        calendar->show();
    } else {
        QMessageBox::warning(this, "Login", "Wrong username and/or password");
    }
}

