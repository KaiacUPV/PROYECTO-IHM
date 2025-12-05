#include "profilewindow.h"
#include "navigation.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

ProfileWindow::ProfileWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Modificar perfil");

    auto &nav = Navigation::instance();
    const QString currentNick = /* Aquí no tenemos el usuario logado globalmente; */ QString();

    m_leNick = new QLineEdit(this);
    m_leNick->setReadOnly(true);

    m_leEmail = new QLineEdit(this);
    m_lePass = new QLineEdit(this);
    m_deBirth = new QDateEdit(this);
    m_deBirth->setCalendarPopup(true);

    m_btnAvatar = new QPushButton("Cambiar avatar...", this);
    m_avatarPreview = new QLabel(this);
    m_avatarPreview->setFixedSize(64,64);
    m_avatarPreview->setFrameShape(QFrame::Box);

    m_btnSave = new QPushButton("Guardar", this);

    auto *form = new QFormLayout;
    form->addRow("Nickname:", m_leNick);
    form->addRow("Email:", m_leEmail);
    form->addRow("Contraseña:", m_lePass);
    form->addRow("Fecha nacimiento:", m_deBirth);
    form->addRow(m_btnAvatar, m_avatarPreview);

    auto *lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addWidget(m_btnSave);

    // Si tienes un usuario "actual", rellénalo aquí:
    // User *u = ...; m_leNick->setText(u->nickName()); ...

    connect(m_btnAvatar, &QPushButton::clicked, this, &ProfileWindow::onChooseAvatar);
    connect(m_btnSave, &QPushButton::clicked, this, &ProfileWindow::onSave);

    resize(420, 260);
}

void ProfileWindow::onChooseAvatar()
{
    QString f = QFileDialog::getOpenFileName(this, "Selecciona avatar", QString(), "Images (*.png *.jpg *.bmp)");
    if (f.isEmpty()) return;
    QImage img(f);
    if (img.isNull()) return;
    m_avatar = img.scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_avatarPreview->setPixmap(QPixmap::fromImage(m_avatar));
}

void ProfileWindow::onSave()
{
    // Debes obtener el User actual, modificarlo y llamar a Navigation::updateUser
    QMessageBox::information(this, "Perfil", "Guardado (implementa la asociación con Navigation).");
}
