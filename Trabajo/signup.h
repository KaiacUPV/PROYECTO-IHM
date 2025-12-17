#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QImage>
#include <QLabel>
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
    void on_btn_avatar_clicked();
    void onTogglePassword1(bool checked);
    void onTogglePassword2(bool checked);

private:
    Ui::signup *ui;
    QImage selectedAvatar;
    void setCircularLabel(QLabel *label, const QPixmap &pixmap, int diameter);
    QPixmap makeRoundedPixmap(const QPixmap &src, int diameter);
};

#endif // SIGNUP_H
