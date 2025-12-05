#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "navigation.h"
#include "navdaoexception.h"

#include <QMessageBox>
#include <QDate>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    refreshView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshView()
{
    ui->textEdit->clear();

    try {
        Navigation &nav = Navigation::instance();

        ui->textEdit->append("Usuarios:\n");
        const auto &users = nav.users();
        for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
            const User &u = it.value();
            ui->textEdit->append(
                QStringLiteral(" - %1 (%2), nacimiento: %3, sesiones: %4")
                    .arg(u.nickName(),
                         u.email(),
                         u.birthdate().toString(Qt::ISODate),
                         QString::number(u.sessions().size())));
        }

        ui->textEdit->append("\nProblemas:\n");
        const auto &problems = nav.problems();
        for (const Problem &p : problems) {
            ui->textEdit->append(QStringLiteral(" - %1").arg(p.text()));
        }

    } catch (const NavDAOException &ex) {
        QMessageBox::critical(this, tr("DB error"), ex.what());
    }
}

void MainWindow::on_reloadButton_clicked()
{
    try {
        Navigation &nav = Navigation::instance();
        nav.reload();
        refreshView();
    } catch (const NavDAOException &ex) {
        QMessageBox::critical(this, tr("DB error"), ex.what());
    }
}

void MainWindow::on_addDummyUserButton_clicked()
{
    try {
        Navigation &nav = Navigation::instance();

        // Crear usuario si no existe
        if (!nav.findUser("alumno")) {
            User u("alumno",
                   "alumno@example.com",
                   "1234",
                   QImage(),
                   QDate(2000, 1, 1));
            nav.addUser(u);
        }

        // Añadir una sesión de prueba usando SIEMPRE Navigation::addSession
        Session s(QDateTime::currentDateTime(), 10, 2);
        nav.addSession("alumno", s);

        refreshView();

    } catch (const NavDAOException &ex) {
        QMessageBox::critical(this, tr("DB error"), ex.what());
    }
}
