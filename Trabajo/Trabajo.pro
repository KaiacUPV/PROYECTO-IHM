QT += core gui svgwidgets sql
CONFIG += c++17

SOURCES += \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    signup.cpp \
    tool.cpp \
    usuario.cpp \
    utils.cpp

HEADERS += \
    lib/include/navdaoexception.h \
    lib/include/navigation.h \
    lib/include/navigationdao.h \
    lib/include/navtypes.h \
    login.h \
    mainwindow.h \
    signup.h \
    tool.h \
    usuario.h \
    utils.h

FORMS += \
    login.ui \
    mainwindow.ui \
    signup.ui \
    usuario.ui

# Agregar esta línea (muy importante)
INCLUDEPATH += $$PWD/lib/include

INCLUDEPATH += $$PWD/Headers
LIBS += -L$$PWD/lib -lnavlib

DISTFILES += \
    resources/carta_nautica.jpg \
    resources/icons/arco.png \
    resources/icons/borrar.png \
    resources/icons/color.png \
    resources/icons/compass_leg.svg \
    resources/icons/linea.png \
    resources/icons/mover.png \
    resources/icons/punto.png \
    resources/icons/ruler.svg \
    resources/icons/texto.png \
    resources/icons/transportador.svg \
    resources/icons/zoom.png

RESOURCES += \
    Resources.qrc
