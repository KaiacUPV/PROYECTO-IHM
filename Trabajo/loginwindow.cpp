#include "loginwindow.h"

loginwindow::loginwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);  // inicializa la interfaz del .ui

    // Opcional: asignar punteros a los widgets del .ui
    lineEdit_user = ui->lineEdit_user;
    lineEdit_pass = ui->lineEdit_pass;
    btn_login = ui->btn_login;

    // Conectar el botón de login al slot
    connect(btn_login, &QPushButton::clicked, this, &loginwindow::onLoginClicked);
}

loginwindow::~loginwindow()
{
    delete ui;  // libera la UI al cerrar la ventana
}

// Ejemplo de slot para manejar login
void loginwindow::onLoginClicked()
{
    QString user = lineEdit_user->text();
    QString pass = lineEdit_pass->text();

    if(user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "Login", "Por favor ingrese usuario y contraseña.");
        return;
    }

    // Aquí podrías verificar credenciales con tu base de datos o lógica
    QMessageBox::information(this, "Login", "Usuario: " + user + "\nContraseña: " + pass);

    // Cerrar la ventana después de login exitoso (opcional)
    // this->close();
}
