#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
    class Login;
}

class Login : public QWidget {
Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

private:
    Ui::Login *ui;

private slots:
    void on_pushButton_login_clicked();

signals:
    void loginSuccessful();

};
#endif // LOGIN_H
