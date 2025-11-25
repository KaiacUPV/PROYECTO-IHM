#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QLabel;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onEmailEditingFinished();
    void onPasswordEditingFinished();
    void onRepeatPasswordEditingFinished();

    void onAcceptClicked();


private:
    Ui::MainWindow *ui;

    bool validEmail;
    bool validPassword;
    bool equalPasswords;

    static const int EQUALS = 0;

    void manageError(QLabel *errorLabel, QLineEdit *edit, bool &flag);
    void manageCorrect(QLabel *errorLabel, QLineEdit *edit, bool &flag);

    void showErrorMessage(QLabel *errorLabel, QLineEdit *edit);
    void hideErrorMessage(QLabel *errorLabel, QLineEdit *edit);

    bool checkEmail();
    bool checkPassword();
    bool checkEquals();

    void updateAcceptEnabled();
};

#endif // MAINWINDOW_H
