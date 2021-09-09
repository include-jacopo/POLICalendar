#include "Login.h"
#include "./ui_Login.h"
#include <QWidget>
#include <QMessageBox> //per mostrare l'errore di login

Login::Login(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Login) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
}

void Login::on_pushButton_login_clicked() {
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (username == "jacopo" && password == "admin") {
        //TODO
    } else {
        QMessageBox::warning(this, "Login", "Wrong username and/or password");
    }
}

