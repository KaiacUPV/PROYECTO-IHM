#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QCryptographicHash>

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
    connect(ui->Cancel_Button, &QPushButton::clicked, this, &login::onCancel);
    connect(ui->btnsingup,     &QToolButton::clicked, this, &login::onSignup);
    connect(ui->btnShowPassword, &QToolButton::toggled,
            this, &login::onTogglePassword);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->btnShowPassword->setCheckable(true);
    ui->btnShowPassword->setIcon(QIcon(":icon/resources/icons/eye-closed.svg"));


    setWindowTitle("Carta Náutica - Iniciar Sesión");

    // --- ESTILO VISUAL MEJORADO (QSS - DARK THEME) ---
    const QString qss = R"(
        QWidget {
            background-color: #1E1E1E;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            font-size: 14px;
            color: #E0E0E0;
        }
        
        /* Campos de texto */
        QLineEdit {
            background-color: #3C3C3C;
            border: 1px solid #555555;
            border-radius: 6px;
            padding: 8px;
            color: #E0E0E0;
            selection-background-color: #264F78;
        }
        QLineEdit:focus {
            border: 1px solid #40A9FF;
        }

        /* Botones principales */
        QPushButton {
            background-color: #0E639C;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #1177BB;
        }
        QPushButton:pressed {
            background-color: #094771;
        }

        /* Botón Cancelar (rojo/gris) */
        QPushButton#Cancel_Button {
            background-color: #D32F2F;
        }
        QPushButton#Cancel_Button:hover {
            background-color: #F44336;
        }

        /* Botón Signup (enlace o secundario) */
        QToolButton#btnsingup {
            background: transparent;
            color: #40A9FF;
            border: none;
            font-weight: bold;
            text-decoration: underline;
        }
        QToolButton#btnsingup:hover {
            color: #69C0FF;
        }

        /* Etiquetas */
        QLabel {
            color: #E0E0E0;
            font-weight: 500;
        }
    )";
    this->setStyleSheet(qss);
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

void login::onCancel()
{
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
