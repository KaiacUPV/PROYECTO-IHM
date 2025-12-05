#pragma once
#include <QWidget>

class QLineEdit;
class QDateEdit;
class QPushButton;
class QLabel;

class ProfileWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileWindow(QWidget *parent = nullptr);

private slots:
    void onChooseAvatar();
    void onSave();

private:
    QLineEdit *m_leNick;
    QLineEdit *m_leEmail;
    QLineEdit *m_lePass;
    QDateEdit *m_deBirth;
    QPushButton *m_btnAvatar;
    QLabel *m_avatarPreview;
    QPushButton *m_btnSave;

    QImage m_avatar;
};
