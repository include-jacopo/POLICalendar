#include "Login.h"
#include "ui_Login.h"
#include "../../Controller/Controller.h"
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
    std::string username = ui->lineEdit_username->text().toStdString();
    std::string password = ui->lineEdit_password->text().toStdString();
    std::string serverUrl = ui->lineEdit_url->text().toStdString();
    int serverPort = ui->lineEdit_port->text().toInt();

    auto controller = Controller::getInstance();
    if (controller->createSession(serverUrl, username, password, serverPort)) {
        emit loginSuccessful();
    } else {
        QMessageBox::warning(this, "Login", "Login fallito");
    }
}
