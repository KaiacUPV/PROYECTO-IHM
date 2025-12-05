#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "navtypes.h"   // <-- NECESARIO para que User exista

class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void userLoggedIn(User *u);   // <-- señal debe ir AQUÍ (no en private)

private slots:
    void onLogin();
    void onRegister();

private:
    QLineEdit *m_leNick;
    QLineEdit *m_lePass;

    QPushButton *m_btnLogin;
    QPushButton *m_btnRegister;
};

#endif // LOGINWINDOW_H
