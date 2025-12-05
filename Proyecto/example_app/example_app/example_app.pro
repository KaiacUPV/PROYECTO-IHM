QT += widgets sql
CONFIG += console
TEMPLATE = app
CONFIG += c++17

TARGET = example_app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Incluir las cabeceras de la librería
INCLUDEPATH += $$PWD/lib/include

# Incluir la librería compilada (libnavlib.a)
LIBS += -L$$PWD/lib -lnavlib







