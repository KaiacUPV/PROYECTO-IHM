#include "registerwindow.h"
#include "navigation.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Registro - Pizarra de navegación");

    m_leNick = new QLineEdit(this);
    m_leEmail = new QLineEdit(this);
    m_lePass = new QLineEdit(this);
    m_lePass->setEchoMode(QLineEdit::Password);

    m_deBirth = new QDateEdit(this);
    m_deBirth->setCalendarPopup(true);
    m_deBirth->setDate(QDate::currentDate().addYears(-20));

    m_btnAvatar = new QPushButton("Elegir avatar...", this);

    m_avatarPreview = new QLabel(this);
    m_avatarPreview->setFixedSize(64, 64);
    m_avatarPreview->setFrameShape(QFrame::Box);
    m_avatarPreview->setAlignment(Qt::AlignCenter);

    m_btnSubmit = new QPushButton("Crear cuenta", this);

    auto *form = new QFormLayout;
    form->addRow("Nickname:", m_leNick);
    form->addRow("Email:", m_leEmail);
    form->addRow("Contraseña:", m_lePass);
    form->addRow("Fecha nacimiento:", m_deBirth);
    form->addRow(m_btnAvatar, m_avatarPreview);

    auto *lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addWidget(m_btnSubmit);

    connect(m_btnAvatar, &QPushButton::clicked, this, &RegisterWindow::onChooseAvatar);
    connect(m_btnSubmit, &QPushButton::clicked, this, &RegisterWindow::onSubmit);

    resize(420, 260);
}

void RegisterWindow::onChooseAvatar()
{
    QString f = QFileDialog::getOpenFileName(
        this,
        "Selecciona avatar",
        QString(),
        "Imágenes (*.png *.jpg *.bmp)"
        );

    if (f.isEmpty())
        return;

    QImage img(f);
    if (img.isNull()) {
        QMessageBox::warning(this, "Error", "La imagen no es válida.");
        return;
    }

    m_avatar = img.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_avatarPreview->setPixmap(QPixmap::fromImage(m_avatar));
}

bool RegisterWindow::validateInput(QString &err)
{
    QString nick = m_leNick->text().trimmed();
    QString email = m_leEmail->text().trimmed();
    QString pass = m_lePass->text();

    if (nick.size() < 6 || nick.size() > 15) {
        err = "El nickname debe tener entre 6 y 15 caracteres.";
        return false;
    }

    if (!nick.contains(QRegularExpression("^[A-Za-z0-9_-]+$"))) {
        err = "El nickname contiene caracteres inválidos.";
        return false;
    }

    QRegularExpression emailRx(R"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
    if (!emailRx.match(email).hasMatch()) {
        err = "Email inválido.";
        return false;
    }

    // contraseña: 8-20, mayúsculas, minúsculas, dígito y caracter especial
    QRegularExpression passRx(
        R"((?=.{8,20}$)(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[!@#\$%&\*\(\)\-+=]).*)"
        );
    if (!passRx.match(pass).hasMatch()) {
        err = "La contraseña debe tener entre 8 y 20 caracteres, con:\n-mayúsculas\n-minúsculas\n-números\n-caracter especial.";
        return false;
    }

    // mínimo 16 años
    QDate birth = m_deBirth->date();
    if (birth.addYears(16) > QDate::currentDate()) {
        err = "Debes tener al menos 16 años.";
        return false;
    }

    return true;
}

void RegisterWindow::onSubmit()
{
    QString err;
    if (!validateInput(err)) {
        QMessageBox::warning(this, "Validación", err);
        return;
    }

    User u(
        m_leNick->text().trimmed(),
        m_leEmail->text().trimmed(),
        m_lePass->text(),
        m_avatar,
        m_deBirth->date()
        );

    try {
        Navigation::instance().addUser(u);
        QMessageBox::information(this, "Registro", "Usuario creado con éxito.");
        close();
    } catch (const NavDAOException &ex) {
        QMessageBox::critical(this, "Error BD", QString::fromStdString(ex.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", "Error inesperado al crear usuario.");
    }
}
