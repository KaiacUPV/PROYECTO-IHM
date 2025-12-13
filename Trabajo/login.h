#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "navigation.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

signals:
    void loginSuccess(const User &user);
    void openSignup();

private slots:
    void onAccept();
    void onCancel();
    void onSignup();
    void onTogglePassword(bool checked);

private:
    Ui::login *ui;
};

#endif // LOGIN_H
