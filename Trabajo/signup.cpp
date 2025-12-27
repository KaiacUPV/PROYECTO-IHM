#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QPainter>
#include <QPainterPath>
#include <QRegularExpression>
#include <QDate>
#include <QFile>
#include <QRegularExpressionMatch>

// HASH
static QString hashPassword(const QString &password)
{
    return QCryptographicHash::hash(
               password.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}
void signup::setCircularLabel(QLabel *label, const QPixmap &pixmap, int diameter)
{
    if (!label) return;
    label->setFixedSize(diameter, diameter);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
    label->setMask(QRegion(0, 0, diameter, diameter, QRegion::Ellipse));
}
QPixmap signup::makeRoundedPixmap(const QPixmap &src, int diameter)
{
    if (src.isNull()) return QPixmap();

    QPixmap scaled = src.scaled(
        diameter,
        diameter,
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
        );

    QPixmap rounded(diameter, diameter);
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.addEllipse(0, 0, diameter, diameter);
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, scaled);

    return rounded;
}

signup::signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signup)
{
    ui->setupUi(this);

    // Cargar estilo específico para Signup
    QFile file(":/resources/signup.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }

    connect(ui->Accept_Button, &QPushButton::clicked, this, &signup::onAccept);
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

    setWindowTitle("Carta Náutica - Registrarse");
}


signup::~signup()
{
    delete ui;
}

bool isValidUsername(const QString &username) {
    if (username.length() < 6 || username.length() > 15) {
        return false;
    }
    if (username.contains(' ')) {
        return false;
    }
    return true;
}

bool isValidPassword(const QString &password) {
    if (password.length() < 8 || password.length() > 20) {
        return false;
    }
    QRegularExpression reUpper("[A-Z]");
    QRegularExpression reLower("[a-z]");
    QRegularExpression reDigit("\\d");
    QRegularExpression reSpecial("[^a-zA-Z0-9]");

    if (!password.contains(reUpper)) {
        return false;
    }
    if (!password.contains(reLower)) {
        return false;
    }
    if (!password.contains(reDigit)) {
        return false;
    }
    if (!password.contains(reSpecial)) {
        return false;
    }

    return true;
}

void signup::onAccept()
{
    QString nick  = ui->txt_nick->text().trimmed();
    QString email = ui->Text_Email->text().trimmed();
    QString pass1 = ui->lineEdit_2->text();
    QString pass2 = ui->lineEdit_3->text();

    if (!isValidUsername(nick)) {
        QMessageBox::warning(this, "Error de validación", "El nombre de usuario debe tener entre 6 y 15 caracteres y no contener espacios.");
        return;
    }

    if (!isValidPassword(pass1)) {
        QMessageBox::warning(this, "Error de validación", "La contraseña debe tener entre 8 y 20 caracteres, e incluir al menos una mayúscula, una minúscula, un dígito y un carácter especial.");
        return;
    }

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
    QImage finalAvatar;

    if (selectedAvatar.isNull()) {
        finalAvatar.load(":/icon/resources/icons/perfil.jpg");
    } else {
        finalAvatar = selectedAvatar;
    }

    QPixmap pixmap = QPixmap::fromImage(finalAvatar);
    QPixmap rounded = makeRoundedPixmap(pixmap, 128);

    setCircularLabel(ui->lblUserAvatar, rounded, 128);


    User newUser(
        nick,
        email,
        hashPassword(pass1),   // 🔐 HASH
        finalAvatar,
        ui->date_birth->date()
        );

    nav.dao().saveUser(newUser);
    nav.addUser(newUser);

    // QMessageBox::information(this, "Correcto", "Usuario creado correctamente.");
    emit signupSuccess(newUser);
    close();
}

void signup::on_btn_avatar_clicked()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        "Seleccionar avatar",
        "",
        "Imágenes (*.png *.jpg *.jpeg)"
        );

    if (!file.isEmpty()) {
        selectedAvatar.load(file);

        QPixmap pixmap = QPixmap::fromImage(selectedAvatar);
        QPixmap rounded = makeRoundedPixmap(pixmap, 128);

        setCircularLabel(ui->lblUserAvatar, rounded, 128);
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
