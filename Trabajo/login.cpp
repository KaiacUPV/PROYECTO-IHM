#include "login.h"
#include "ui_login.h"
#include "navigation.h"
#include <QMessageBox>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    connect(ui->Accept_Button, &QPushButton::clicked, this, &login::onAccept);
    connect(ui->Cancel_Button, &QPushButton::clicked, this, &login::onCancel);
    connect(ui->btnsingup,     &QToolButton::clicked, this, &login::onSignup);

    setWindowTitle("Carta Náutica - IniciarS Sesión");
}

login::~login()
{
    delete ui;
}

void login::onAccept()
{
    QString emailOrNick = ui->Text_Email->text().trimmed();
    QString pass         = ui->lineEdit_2->text();

    if (emailOrNick.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes rellenar todos los campos.");
        return;
    }

    auto &nav = Navigation::instance();
    const auto &usersMap = nav.users(); // QMap<QString, User>

    bool found = false;
    User foundUser;

    QString input = emailOrNick.trimmed().toLower();

    for (const User &u : usersMap) {
        if (u.nickName().toLower() == input || u.email().toLower() == input) {
            foundUser = u;
            found = true;
            break;
        }
    }


    if (!found) {
        QMessageBox::critical(this, "Error", "El usuario no existe.");
        return;
    }

    if (foundUser.password() != pass) {
        QMessageBox::critical(this, "Error", "Contraseña incorrecta.");
        return;
    }

    // Éxito
    QMessageBox::information(this,"Correcto","Inicio de sesión correcto.");
    emit loginSuccess(foundUser);
    close();
}

void login::onCancel()
{
    close();
}

void login::onSignup()
{
    emit openSignup();
    login::~login();
}
