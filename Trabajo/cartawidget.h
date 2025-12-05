#ifndef CARTAWIDGET_H
#define CARTAWIDGET_H

#include <QWidget>
#include <QList>

class CartaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CartaWidget(QWidget *parent = nullptr);

    enum Herramienta {
        Ninguna,
        Texto,
        Punto,
        Linea,
        Arco,
        Color,
        Borrar,
        Mover,
        Regla,
        Zoom
    };

    void setModo(Herramienta h);
    void clearAll();


protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void paintEvent(QPaintEvent *ev) override;

private:
    Herramienta m_modo;
    QPixmap m_mapa;
};

#endif
