#ifndef LOGIN_H
#define LOGIN_H

#include <QFrame>

namespace Ui {
    class Login;
}

class Login : public QFrame {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

private:
    Ui::Login *ui;

private slots:
    void onLoginClicked();

signals:
    void loginSuccessful();
};

#endif // LOGIN_H
