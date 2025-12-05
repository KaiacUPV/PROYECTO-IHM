QT += core gui widgets sql
CONFIG += c++17

SOURCES += \
    cartawidget.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    problemwindow.cpp \
    profilewindow.cpp \
    registerwindow.cpp \
    resultwindow.cpp

HEADERS += \
    cartawidget.h \
    lib/include/navdaoexception.h \
    lib/include/navigation.h \
    lib/include/navigationdao.h \
    lib/include/navtypes.h \
    loginwindow.h \
    mainwindow.h \
    problemwindow.h \
    profilewindow.h \
    registerwindow.h \
    resultwindow.h

FORMS += \
    LoginWindow.ui \
    RegisterWindow.ui \
    mainwindow.ui

# Agregar esta línea (muy importante)
INCLUDEPATH += $$PWD/lib/include

INCLUDEPATH += $$PWD/Headers
LIBS += -L$$PWD/lib -lnavlib

DISTFILES += \
    resources/carta_nautica.jpg \
    resources/icons/compass_leg.svg \
    resources/icons/ruler.svg \
    resources/icons/transportador.svg

RESOURCES += \
    Resources.qrc
