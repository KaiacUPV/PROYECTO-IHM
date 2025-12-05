#include "navigation.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Navigation &nav = Navigation::instance();
    User *u = nav.authenticate("demo", "1234");

    if (u) {
        qDebug() << "Usuario autenticado:" << u->getName();
    } else {
        qDebug() << "Error de autenticación";
    }

    return app.exec();
}
