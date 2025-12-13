#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include "navigation.h"

namespace Ui {
class signup;
}

class signup : public QWidget
{
    Q_OBJECT

public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();

signals:
    void signupSuccess(const User &user);

private slots:
    void onAccept();
    void onCancel();
    void on_btn_avatar_clicked();

private:
    Ui::signup *ui;
    QImage selectedAvatar;

};

#endif // SIGNUP_H
