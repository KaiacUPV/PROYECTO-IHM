#include "signup.h"
#include "ui_signup.h"
#include "navigation.h"
#include <QMessageBox>
#include <QDate>

signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);

    connect(ui->Accept_Button, &QPushButton::clicked, this, &signup::onAccept);
    connect(ui->Cancel_Button, &QPushButton::clicked, this, &signup::onCancel);

    ui->lblUserAvatar->setPixmap(QPixmap(":icon/resources/icons/perfil.jpg"));
    ui->lblUserAvatar->setScaledContents(true);

    setWindowTitle("Carta Náutica - Registrarse");
}

signup::~signup()
{
    delete ui;
}

void signup::onAccept()
{
    QString email = ui->Text_Email->text().trimmed();
    QString pass1 = ui->lineEdit_2->text();
    QString pass2 = ui->lineEdit_3->text();

    if (email.isEmpty() || pass1.isEmpty() || pass2.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes rellenar todos los campos.");
        return;
    }

    if (pass1 != pass2) {
        QMessageBox::warning(this, "Error", "Las contraseñas no coinciden.");
        return;
    }

    auto &nav = Navigation::instance();
    const auto &usersMap = nav.users();

    // Comprobar existencia por nick o email
    for (const User &u : usersMap) {
        if (u.nickName() == email || u.email() == email) {
            QMessageBox::warning(this,"Error","Este usuario ya existe.");
            return;
        }
    }

    // Crear usuario: nickname = email (puedes cambiar esto a otro campo si tienes uno en UI)
    User newUser(
        email,                // nickName
        email,                // email
        pass1,                // password
        QImage(),             // avatar vacío
        QDate::currentDate()  // birthdate (fecha actual)
        );

    // Guardar en BD: saveUser espera User& en tu header
    // Nota: saveUser modifica el objeto (setInsertedInDb), así que pasamos una variable no-const
    nav.dao().saveUser(newUser);

    // También actualizar la colección en memoria (Navigation mantiene su propio mapa).
    // Si Navigation::addUser existe, úsalo. De lo contrario, recarga:
    try {
        nav.addUser(newUser);
    } catch (...) {
        // si no existe addUser o hay problemas - simplemente recargamos desde BD
        nav.reload();
    }

    QMessageBox::information(this, "Correcto", "Usuario creado correctamente.");
    emit signupSuccess();
    close();
}

void signup::onCancel()
{
    close();
}
