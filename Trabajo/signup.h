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
    void signupSuccess();

private slots:
    void onAccept();
    void onCancel();

private:
    Ui::signup *ui;
};

#endif // SIGNUP_H
