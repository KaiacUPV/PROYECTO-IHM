#include "mainwindow.h"
#include "navigation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load the DB into Navigation singleton before UI shows
    try {
        Navigation::instance().reload();
    } catch (...) {
        // If reload fails, continue — UI can show errors where needed
    }

    MainWindow w;
    w.showMaximized();  // abre en pantalla completa


    return a.exec();
}
