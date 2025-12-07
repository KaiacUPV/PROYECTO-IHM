#ifndef USUARIO_H
#define USUARIO_H

#include <QWidget>

namespace Ui {
class usuario;
}

class usuario : public QWidget
{
    Q_OBJECT

public:
    explicit usuario(QWidget *parent = nullptr);
    ~usuario();

private:
    Ui::usuario *ui;
};

#endif // USUARIO_H
