#include "mainwindow.h"
#include "navigation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Carga la BD en el singleton de Navegación antes de que se muestre la interfaz de usuario
    try {
        Navigation::instance().reload();
    } catch (...) {
        // Si la recarga falla, continúa — la interfaz de usuario puede mostrar errores donde sea necesario
    }

    MainWindow w;
    w.showMaximized();  // abre en pantalla completa


    return a.exec();
}
