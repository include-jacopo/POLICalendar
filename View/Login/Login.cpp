#include "Login.h"
#include "ui_Login.h"
#include "../../Controller/Controller.h"
#include <QMessageBox>
#include <QDir>

Login::Login(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::Login) {
    ui->setupUi(this);

    // Set logo image
    auto logo = QPixmap(":/Login/logo.png").scaledToWidth(this->width(), Qt::SmoothTransformation);
    ui->logo->setPixmap(logo);

    // Style
    setStyleSheet("Login {background-color: white;}\n"
                  "QLabel#logo {margin-bottom: 8px;}");

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
    int serverPort = ui->lineEdit_port->text().toInt(); // Returns 0 if conversion fails, ok for empty field

    // Check URL protocol
    if (!(serverUrl.starts_with("http://") || serverUrl.starts_with("https://"))) {
        QMessageBox::warning(this, "Login", "Inserire protocollo nell'URL del server (http:// o https://)");
        return;
    }

    // Check port
    if (serverPort < 0 || serverPort > 65535) {
        QMessageBox::warning(this, "Login", "Porta non valida");
        return;
    }

    auto controller = Controller::getInstance();
    int status = controller->createSession(serverUrl, username, password, serverPort);
    QString errorMsg;
    switch (status) {
        case 0:
            emit loginSuccessful();
            return;
        case 1:
            errorMsg = "Username o password errati";
            break;
        case 2:
            errorMsg = "Connessione al server fallita";
            break;
        case 3:
            errorMsg = "Timeout di connessione";
            break;
        default:
            errorMsg = "Errore";
    }
    if (!errorMsg.isEmpty()) {
        QMessageBox::warning(this, "Login", errorMsg);
    }
}
