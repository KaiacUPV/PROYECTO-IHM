#include "cartawidget.h"
#include <QPainter>
#include <QMouseEvent>

CartaWidget::CartaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_modo = Ninguna;

    // Cargar la imagen desde recursos
    m_mapa = QPixmap(":/resources/carta_nautica.jpg");

    if (m_mapa.isNull())
        qDebug() << "ERROR: No se pudo cargar carta_nautica.jpg";
}
void CartaWidget::setModo(Herramienta h)
{
    m_modo = h;
}

void CartaWidget::mousePressEvent(QMouseEvent *ev)
{
    // TEMPORAL PARA QUE NO DE ERRORES
    qDebug("Click en modo: %d", m_modo);
    QWidget::mousePressEvent(ev);
}

void CartaWidget::clearAll()
{
    // cuando tengas marcas, puntos, líneas, etc. se borrarán aquí.
    // por ahora solo repinta
    update();
}


void CartaWidget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    // Dibujar imagen escalada al tamaño del widget
    if (!m_mapa.isNull())
        p.drawPixmap(rect(), m_mapa);

    // aquí podrás dibujar puntos, líneas, textos, etc
}

