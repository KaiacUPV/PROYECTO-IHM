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
    connect(ui->btnsingup,     &QToolButton::clicked, this, &login::onSignup);
    connect(ui->btnShowPassword, &QToolButton::toggled,
            this, &login::onTogglePassword);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->btnShowPassword->setCheckable(true);
    ui->btnShowPassword->setIcon(QIcon(":icon/resources/icons/eye-closed.svg"));


    setWindowTitle("Carta Náutica - Iniciar Sesión");

    // --- ESTILO VISUAL MEJORADO (QSS - APPLE STYLE DARK) ---
    const QString qss = R"(
        QWidget {
            background-color: #1E1E1E;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
            font-size: 14px;
            color: #E0E0E0;
        }
        
        /* Campos de texto */
        QLineEdit {
            background-color: #2C2C2E; /* Apple Dark Gray */
            border: 1px solid #3A3A3C;
            border-radius: 10px;       /* Rounded corners */
            padding: 10px 12px;
            color: #FFFFFF;
            selection-background-color: #0A84FF;
        }
        QLineEdit:focus {
            border: 1px solid #0A84FF; /* Apple Blue Focus */
            background-color: #3A3A3C;
        }

        /* Botones principales */
        QPushButton {
            background-color: #0A84FF; /* Apple Blue */
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 24px;
            font-weight: 600;
            font-size: 15px;
        }
        QPushButton:hover {
            background-color: #0077ED;
        }
        QPushButton:pressed {
            background-color: #0062C3;
        }

        /* Botón Signup (enlace o secundario) */
        QToolButton#btnsingup {
            background: transparent;
            color: #0A84FF;
            border: none;
            font-weight: 500;
            font-size: 13px;
        }
        QToolButton#btnsingup:hover {
            color: #409CFF;
            text-decoration: underline;
        }

        /* Botón Mostrar Contraseña */
        QToolButton#btnShowPassword {
            background-color: #2C2C2E;
            border: 1px solid #3A3A3C;
            border-radius: 10px;
            min-width: 40px;
            min-height: 35px;
        }
        QToolButton#btnShowPassword:hover {
            background-color: #3A3A3C;
            border: 1px solid #0A84FF;
        }
        QToolButton#btnShowPassword:checked {
            background-color: #3A3A3C;
            border: 1px solid #0A84FF;
            background-color: rgba(10, 132, 255, 0.15);
        }

        /* Etiquetas */
        QLabel {
            color: #E0E0E0;
            font-weight: 400;
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
