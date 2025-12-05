#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QImage>

class QLineEdit;
class QDateEdit;
class QLabel;
class QPushButton;

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);

private slots:
    void onChooseAvatar();
    void onSubmit();

private:
    bool validateInput(QString &err);

    QLineEdit *m_leNick;
    QLineEdit *m_leEmail;
    QLineEdit *m_lePass;
    QDateEdit *m_deBirth;

    QPushButton *m_btnAvatar;
    QLabel *m_avatarPreview;
    QPushButton *m_btnSubmit;

    QImage m_avatar;
};

#endif // REGISTERWINDOW_H
