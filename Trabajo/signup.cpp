#include "signup.h"
#include "ui_signup.h"
#include "navigation.h"
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
#include <QRegularExpression>

signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);

    connect(ui->Accept_Button, &QPushButton::clicked, this, &signup::onAccept);
    connect(ui->Cancel_Button, &QPushButton::clicked, this, &signup::onCancel);
    connect(ui->Btn_Avatar, &QPushButton::clicked, this, &signup::on_btn_avatar_clicked );

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
    QString nick = ui->txt_nick->text().trimmed();
    QString email = ui->Text_Email->text().trimmed();
    QString pass1 = ui->lineEdit_2->text();
    QString pass2 = ui->lineEdit_3->text();
    QDate birth = ui->date_birth->date();

    // Campos obligatorios
    if (nick.isEmpty() || email.isEmpty() || pass1.isEmpty() || pass2.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes rellenar todos los campos obligatorios.");
        return;
    }

    // nickname: 6..15 caracteres, letras/dígitos/guion/underscore, sin espacios
    QRegularExpression nickRegex("^[A-Za-z0-9_-]{6,15}$");
    if (!nickRegex.match(nick).hasMatch()) {
        QMessageBox::warning(this, "Error",
                             "El nombre de usuario debe tener entre 6 y 15 caracteres. "
                             "Solo letras, dígitos, guiones y guiones bajos están permitidos.");
        return;
    }

    // Contraseñas iguales
    if (pass1 != pass2) {
        QMessageBox::warning(this, "Error", "Las contraseñas no coinciden.");
        return;
    }

    // Contraseña: 8..20, al menos una mayúscula, minúscula, dígito y caracter especial (!@#$%&*()-+=)
    if (pass1.length() < 8 || pass1.length() > 20) {
        QMessageBox::warning(this, "Error", "La contraseña debe tener entre 8 y 20 caracteres.");
        return;
    }
    if (!QRegularExpression("[A-Z]").match(pass1).hasMatch()) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos una letra mayúscula.");
        return;
    }
    if (!QRegularExpression("[a-z]").match(pass1).hasMatch()) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos una letra minúscula.");
        return;
    }
    if (!QRegularExpression("[0-9]").match(pass1).hasMatch()) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos un dígito.");
        return;
    }
    if (!QRegularExpression(R"([!@#$%&*\(\)\-+=])").match(pass1).hasMatch()) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos un carácter especial (!@#$%&*()-+=).");
        return;
    }

    // Email (formato)
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Error", "El correo electrónico no tiene un formato válido.");
        return;
    }

    // Edad > 16 años
    QDate today = QDate::currentDate();
    int age = today.year() - birth.year();
    if (today.month() < birth.month() || (today.month() == birth.month() && today.day() < birth.day())) {
        age--;
    }
    if (age <= 16) {
        QMessageBox::warning(this, "Error", "Debes tener más de 16 años para registrarte.");
        return;
    }

    // Avatar elegido o por defecto
    QImage avatarFinal;
    if (selectedAvatar.isNull()) {
        avatarFinal.load(":/icon/resources/icons/perfil.jpg");
    } else {
        avatarFinal = selectedAvatar;
    }

    // Verificar usuario/email no usados
    auto &nav = Navigation::instance();
    const auto &usersMap = nav.users();

    for (const User &u : usersMap) {
        if (u.nickName().compare(nick, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Error", "El nombre de usuario ya está en uso.");
            return;
        }
        if (u.email().compare(email, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Error", "El correo electrónico ya está registrado.");
            return;
        }
    }

    // Crear y persistir usuario
    User newUser(
        nick,         // nickName
        email,        // email
        pass1,        // password
        avatarFinal,  // avatar
        birth         // birthdate
    );

    try {
        nav.dao().saveUser(newUser); // persistir
        nav.addUser(newUser);        // agregar a memoria si procede

        QMessageBox::information(this, "Correcto", "Usuario creado correctamente.");
        emit signupSuccess();
        close();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("No se pudo guardar el usuario: ") + QString::fromStdString(e.what()));
    }
}

void signup::onCancel()
{
    close();
}
void signup::on_btn_avatar_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Seleccionar avatar",
        "",
        "Imágenes (*.png *.jpg *.jpeg *.bmp)"
        );

    if (!fileName.isEmpty()) {
        selectedAvatar.load(fileName);

        ui->lblUserAvatar->setPixmap(
            QPixmap::fromImage(selectedAvatar).scaled(128,128, Qt::KeepAspectRatio)
            );
    }
}
