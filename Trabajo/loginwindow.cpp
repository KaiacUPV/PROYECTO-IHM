#include "loginwindow.h"
#include "registerwindow.h"
#include "mainwindow.h"
#include "navigation.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Login - Pizarra de navegación");

    m_leNick = new QLineEdit(this);
    m_lePass = new QLineEdit(this);
    m_lePass->setEchoMode(QLineEdit::Password);

    m_btnLogin = new QPushButton("Entrar", this);
    m_btnRegister = new QPushButton("Registrarse", this);

    auto *form = new QFormLayout;
    form->addRow("Nickname:", m_leNick);
    form->addRow("Contraseña:", m_lePass);

    auto *lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addWidget(m_btnLogin);
    lay->addWidget(m_btnRegister);

    connect(m_btnLogin, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(m_btnRegister, &QPushButton::clicked, this, &LoginWindow::onRegister);

    resize(350, 180);
}

void LoginWindow::onLogin()
{
    QString nick = m_leNick->text().trimmed();
    QString pass = m_lePass->text();

    try {
        User *u = Navigation::instance().authenticate(nick, pass);

        if (!u) {
            QMessageBox::warning(this, "Error", "Nickname o contraseña incorrectos.");
            return;
        }

        emit userLoggedIn(u);  // <-- ya funciona
        close();

    } catch (const NavDAOException &ex) {
        QMessageBox::critical(this, "DB Error", QString::fromStdString(ex.what()));
    }
}

void LoginWindow::onRegister()
{
    RegisterWindow *rw = new RegisterWindow();
    rw->setAttribute(Qt::WA_DeleteOnClose);
    rw->show();
    close();
}
