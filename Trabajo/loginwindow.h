#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>

#include "ui_loginwindow.h"  // generado automáticamente por Qt a partir de loginwindow.ui

QT_BEGIN_NAMESPACE
namespace Ui { class loginwindow; }
QT_END_NAMESPACE

class loginwindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);
    ~loginwindow();

private slots:
    void onLoginClicked();  // ejemplo: slot para botón de login

private:
    Ui::loginwindow *ui;   // puntero a la UI generada por Qt

    // Ejemplo: campos internos de login (opcional, porque ya están en el .ui)
    QLineEdit *lineEdit_user;
    QLineEdit *lineEdit_pass;
    QPushButton *btn_login;
};

#endif // LOGINWINDOW_H
