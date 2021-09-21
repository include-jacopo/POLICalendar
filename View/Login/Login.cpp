#include "Login.h"
#include "ui_Login.h"
#include <QMessageBox>
#include <QDir>

Login::Login(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::Login) {
    ui->setupUi(this);

    // Style
    setStyleSheet("Login {background-color: white;}");

    // Set focus
    ui->lineEdit_username->setFocus();
}

Login::~Login() {
    delete ui;
}

void Login::onLoginClicked() {
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (username == "jacopo" && password == "admin") { // TODO TEMP
        emit loginSuccessful();
    } else {
        QMessageBox::warning(this, "Login", "Username o password errati");
    }
}
