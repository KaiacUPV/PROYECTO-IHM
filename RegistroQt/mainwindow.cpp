#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "mainappwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , validEmail(false)
    , validPassword(false)
    , equalPasswords(false)
{
    ui->setupUi(this);

    // Etiquetas de error ocultas al inicio
    ui->lErrorEmail->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);


    connect(ui->Text_Email, &QLineEdit::editingFinished,//Email
            this, &MainWindow::onEmailEditingFinished);

    connect(ui->lineEdit_2, &QLineEdit::editingFinished,//Contreseña1
            this, &MainWindow::onPasswordEditingFinished);

    connect(ui->lineEdit_3, &QLineEdit::editingFinished,//Contraseña2
            this, &MainWindow::onRepeatPasswordEditingFinished);


    // Botones
    connect(ui->Accept_Button, &QPushButton::clicked,
            this, &MainWindow::onAcceptClicked);

    connect(ui->Cancel_Button, &QPushButton::clicked,
            this, &MainWindow::close);


    // Conexión de señales de edición finalizada
    //connect(sender, &QLineEdit::editingFinished,this, &MainWindow::slot );


    // Botones
    //connect(boton, &QPushButton::clicked,this,&MainWindow::slot);

    // Estado inicial
    updateAcceptEnabled();
    //primerObjetoEditar->setFocus();
}

MainWindow::~MainWindow()
{

    delete ui;
}

// ======= Gestión de estados y estilo =======

void MainWindow::manageError(QLabel *errorLabel, QLineEdit *edit, bool &flag)
{
    flag = false;
    showErrorMessage(errorLabel, edit);
    edit->setFocus();
    updateAcceptEnabled();
}

void MainWindow::manageCorrect(QLabel *errorLabel, QLineEdit *edit, bool &flag)
{
    flag = true;
    hideErrorMessage(errorLabel, edit);
    updateAcceptEnabled();
}

void MainWindow::showErrorMessage(QLabel *errorLabel, QLineEdit *edit)
{
    if (errorLabel) {
        errorLabel->setVisible(true);
    }
    if (edit) {
        edit->setStyleSheet("background-color: #FCE5E0;");
    }
}

void MainWindow::hideErrorMessage(QLabel *errorLabel, QLineEdit *edit)
{
    if (errorLabel) {
        errorLabel->setVisible(false);
    }
    if (edit) {
        edit->setStyleSheet("");
    }
}

// ======= Validaciones =======

bool MainWindow::checkEmail()
{
    const QString value = ui->Text_Email->text();

    if (!Utils::checkEmail(value)) {
        ui->lErrorEmail->setText("Email incorrecto");
        ui->lErrorEmail->setVisible(true);
        ui->lErrorEmail->setStyleSheet("color: red;");
        ui->Text_Email->setStyleSheet("background-color: #BFBFBF;");
        return false;
    }

    ui->lErrorEmail->setVisible(false);
    ui->Text_Email->setStyleSheet("");
    return true;
}

bool MainWindow::checkPassword()
{
    const QString value = ui->lineEdit_2->text();

    if (!Utils::checkPassword(value)) {
        ui->label_2->setText("Contraseña no válida");
        ui->label_2->setVisible(true);
        ui->label_2->setStyleSheet("color: red;");
        ui->lineEdit_2->setStyleSheet("background-color: #BFBFBF;");
        return false;
    }

    ui->label_2->setVisible(false);
    ui->lineEdit_2->setStyleSheet("");
    return true;
}

bool MainWindow::checkEquals()
{
    QString pass1 = ui->lineEdit_2->text();
    QString pass2 = ui->lineEdit_3->text();

    if (pass1 != pass2) {
        ui->label_3->setText("Las contraseñas no coinciden");
        ui->label_3->setVisible(true);
        ui->label_3->setStyleSheet("color: red;");
        ui->lineEdit_3->setStyleSheet("background-color: #BFBFBF;");
        return false;
    }

    ui->label_3->setVisible(false);
    ui->lineEdit_3->setStyleSheet("");
    return true;
}



// ======= Slots de edición finalizada =======

void MainWindow::onEmailEditingFinished()
{
    checkEmail();
}

void MainWindow::onPasswordEditingFinished()
{
    updateAcceptEnabled();
}

void MainWindow::onRepeatPasswordEditingFinished()
{
    updateAcceptEnabled();
}




// ======= Botones =======

void MainWindow::onAcceptClicked()
{
    bool okEmail = checkEmail();
    bool okPass = checkPassword();
    bool okEqual = checkEquals();

    if (okEmail && okPass && okEqual) {
        // Crear la ventana principal (Beta aun por ver)
        MainAppWindow *appWindow = new MainAppWindow();
        appWindow->show();

        // Cerrar la ventana de registro
        this->close();
    } else {
        QMessageBox::warning(this, "Error", "Corrige los campos marcados en rojo.");
    }
}



// ======= Habilitar / deshabilitar botón Aceptar =======

void MainWindow::updateAcceptEnabled()
{
    //qDebug() << validEmail << validPassword << equalPasswords;

    //bool allValid = validEmail && validPassword && equalPasswords;
    //ui->Accept_Button->setEnabled(allValid);

    bool allFilled =
        !ui->Text_Email->text().isEmpty() &&
        !ui->lineEdit_2->text().isEmpty() &&
        !ui->lineEdit_3->text().isEmpty();

    ui->Accept_Button->setEnabled(allFilled);
}
