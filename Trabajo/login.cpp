#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFile>

// HASH
static QString hashPassword(const QString &password)
{
    return QCryptographicHash::hash(
               password.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    connect(ui->Accept_Button, &QPushButton::clicked, this, &login::onAccept);
    connect(ui->btnsingup,     &QToolButton::clicked, this, &login::onSignup);
    connect(ui->btnShowPassword, &QToolButton::toggled,
            this, &login::onTogglePassword);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->btnShowPassword->setCheckable(true);
    ui->btnShowPassword->setIcon(QIcon(":icon/resources/icons/eye-closed.svg"));


    setWindowTitle("Carta Náutica - Iniciar Sesión");

    // --- ESTILO VISUAL MEJORADO (QSS - APPLE STYLE DARK) ---
    QFile file(":/resources/login.qss");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        this->setStyleSheet(qss);
    }
}

login::~login()
{
    delete ui;
}

void login::onAccept()
{
    QString input = ui->Text_Email->text().trimmed().toLower();
    QString pass  = ui->lineEdit_2->text();

    if (input.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes rellenar todos los campos.");
        return;
    }

    auto &nav = Navigation::instance();
    const auto &usersMap = nav.users();

    User foundUser;
    bool found = false;

    for (const User &u : usersMap) {
        if (u.nickName().toLower() == input ||
            u.email().toLower() == input) {
            foundUser = u;
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::critical(this, "Error", "El usuario no existe.");
        return;
    }

    if (foundUser.password() != hashPassword(pass)) {
        QMessageBox::critical(this, "Error", "Contraseña incorrecta.");
        return;
    }

    // QMessageBox::information(this, "Correcto", "Inicio de sesión correcto.");
    emit loginSuccess(foundUser);
    close();
}

void login::onSignup()
{
    emit openSignup();
    close();
}

void login::onTogglePassword(bool checked)
{
    ui->lineEdit_2->setEchoMode(
        checked ? QLineEdit::Normal : QLineEdit::Password
        );
    ui->btnShowPassword->setIcon(
        QIcon(checked ? ":icon/resources/icons/eye-open.png" : ":icon/resources/icons/eye-closed.svg")
        );
}
