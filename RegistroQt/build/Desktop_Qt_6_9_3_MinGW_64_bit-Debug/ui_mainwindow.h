/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *Title_Label;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLabel *leEmail;
    QLineEdit *lineEdit_3;
    QLabel *Password_Label;
    QLabel *label_3;
    QLineEdit *Text_Email;
    QLabel *lErrorEmail;
    QLabel *Repeat_Label;
    QPushButton *Cancel_Button;
    QLineEdit *lineEdit_2;
    QPushButton *Accept_Button;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *spacer;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(532, 387);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setObjectName("gridLayout");
        Title_Label = new QLabel(centralWidget);
        Title_Label->setObjectName("Title_Label");
        QFont font;
        font.setPointSize(48);
        Title_Label->setFont(font);

        gridLayout->addWidget(Title_Label, 0, 0, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName("widget");
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        QFont font1;
        font1.setPointSize(12);
        label_2->setFont(font1);

        gridLayout_2->addWidget(label_2, 3, 2, 1, 2);

        leEmail = new QLabel(widget);
        leEmail->setObjectName("leEmail");
        leEmail->setFont(font1);

        gridLayout_2->addWidget(leEmail, 0, 0, 1, 1);

        lineEdit_3 = new QLineEdit(widget);
        lineEdit_3->setObjectName("lineEdit_3");

        gridLayout_2->addWidget(lineEdit_3, 5, 0, 1, 1);

        Password_Label = new QLabel(widget);
        Password_Label->setObjectName("Password_Label");
        Password_Label->setFont(font1);

        gridLayout_2->addWidget(Password_Label, 2, 0, 1, 4);

        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setFont(font1);

        gridLayout_2->addWidget(label_3, 5, 2, 1, 2);

        Text_Email = new QLineEdit(widget);
        Text_Email->setObjectName("Text_Email");

        gridLayout_2->addWidget(Text_Email, 1, 0, 1, 3);

        lErrorEmail = new QLabel(widget);
        lErrorEmail->setObjectName("lErrorEmail");
        lErrorEmail->setFont(font1);

        gridLayout_2->addWidget(lErrorEmail, 1, 3, 1, 2);

        Repeat_Label = new QLabel(widget);
        Repeat_Label->setObjectName("Repeat_Label");
        Repeat_Label->setFont(font1);

        gridLayout_2->addWidget(Repeat_Label, 4, 0, 1, 3);

        Cancel_Button = new QPushButton(widget);
        Cancel_Button->setObjectName("Cancel_Button");
        Cancel_Button->setFont(font1);

        gridLayout_2->addWidget(Cancel_Button, 6, 4, 1, 1);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName("lineEdit_2");

        gridLayout_2->addWidget(lineEdit_2, 3, 0, 1, 1);

        Accept_Button = new QPushButton(widget);
        Accept_Button->setObjectName("Accept_Button");
        Accept_Button->setFont(font1);

        gridLayout_2->addWidget(Accept_Button, 6, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 5, 1, 1, 1);


        gridLayout->addWidget(widget, 2, 0, 1, 1);

        spacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(spacer, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 532, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Formulario de registro", nullptr));
        Title_Label->setText(QCoreApplication::translate("MainWindow", "Sign Up", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        leEmail->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        Password_Label->setText(QCoreApplication::translate("MainWindow", "Password (Debe contener de 8-15 y tener letra y n\303\272mero)", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        lErrorEmail->setText(QCoreApplication::translate("MainWindow", "Text_Label", nullptr));
        Repeat_Label->setText(QCoreApplication::translate("MainWindow", "Repeat Password", nullptr));
        Cancel_Button->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        Accept_Button->setText(QCoreApplication::translate("MainWindow", "Accept", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
