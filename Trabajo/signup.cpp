#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDate>

// HASH
static QString hashPassword(const QString &password)
{
    return QCryptographicHash::hash(
               password.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}

signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);

    connect(ui->Accept_Button, &QPushButton::clicked, this, &signup::onAccept);
    connect(ui->Cancel_Button, &QPushButton::clicked, this, &signup::onCancel);
    connect(ui->Btn_Avatar, &QPushButton::clicked, this, &signup::on_btn_avatar_clicked);

    ui->btnShowPass1->setCheckable(true);
    ui->btnShowPass2->setCheckable(true);

    connect(ui->btnShowPass1, &QToolButton::toggled,
            this, &signup::onTogglePassword1);
    connect(ui->btnShowPass2, &QToolButton::toggled,
            this, &signup::onTogglePassword2);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

    ui->btnShowPass1->setIcon(QIcon(":icon/resources/icons/eye-closed.svg"));
    ui->btnShowPass2->setIcon(QIcon(":icon/resources/icons/eye-closed.svg"));

    ui->btnShowPass1->setAutoRaise(true);
    ui->btnShowPass2->setAutoRaise(true);

    setWindowTitle("Carta Náutica - Registrarse");
}


signup::~signup()
{
    delete ui;
}

void signup::onAccept()
{
    QString nick  = ui->txt_nick->text().trimmed();
    QString email = ui->Text_Email->text().trimmed();
    QString pass1 = ui->lineEdit_2->text();
    QString pass2 = ui->lineEdit_3->text();

    if (nick.isEmpty() || email.isEmpty() || pass1.isEmpty()) {
        QMessageBox::warning(this, "Error", "Campos obligatorios vacíos.");
        return;
    }

    if (pass1 != pass2) {
        QMessageBox::warning(this, "Error", "Las contraseñas no coinciden.");
        return;
    }

    auto &nav = Navigation::instance();
    for (const User &u : nav.users()) {
        if (u.nickName().compare(nick, Qt::CaseInsensitive) == 0 ||
            u.email().compare(email, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Error", "Usuario o email ya registrado.");
            return;
        }
    }

    User newUser(
        nick,
        email,
        hashPassword(pass1),   // 🔐 HASH
        selectedAvatar,
        ui->date_birth->date()
        );

    nav.dao().saveUser(newUser);
    nav.addUser(newUser);

    QMessageBox::information(this, "Correcto", "Usuario creado correctamente.");
    emit signupSuccess(newUser);
    close();
}

void signup::onCancel()
{
    close();
}

void signup::on_btn_avatar_clicked()
{
    QString file = QFileDialog::getOpenFileName(
        this, "Seleccionar avatar", "",
        "Imágenes (*.png *.jpg *.jpeg)"
        );

    if (!file.isEmpty()) {
        selectedAvatar.load(file);
        ui->lblUserAvatar->setPixmap(QPixmap::fromImage(selectedAvatar));
    }
}

void signup::onTogglePassword1(bool checked)
{
    ui->lineEdit_2->setEchoMode(
        checked ? QLineEdit::Normal : QLineEdit::Password
        );
    ui->btnShowPass1->setIcon(
        QIcon(checked ? ":icon/resources/icons/eye-open.png" : ":icon/resources/icons/eye-closed.svg")
        );
}

void signup::onTogglePassword2(bool checked)
{
    ui->lineEdit_3->setEchoMode(
        checked ? QLineEdit::Normal : QLineEdit::Password
        );
    ui->btnShowPass2->setIcon(
        QIcon(checked ? ":icon/resources/icons/eye-open.png" : ":icon/resources/icons/eye-closed.svg")
        );
}
