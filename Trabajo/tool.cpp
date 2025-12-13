#include "tool.h"

#include <QtMath>
#include <QtGlobal>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QSvgRenderer>

Tool::Tool(const QString& svgResourcePath, QGraphicsItem* parent)
    : QGraphicsSvgItem(svgResourcePath, parent)
{
    // Flags para poder moverla, seleccionarla y que ignore las transformaciones del view
    setFlags(QGraphicsItem::ItemIsMovable
             | QGraphicsItem::ItemIsSelectable
             | QGraphicsItem::ItemSendsGeometryChanges
             | QGraphicsItem::ItemIgnoresTransformations);

    // Origen de rotación = centro del SVG
    updateOrigin();

    // Tamaño inicial = tamaño natural del SVG
    m_targetSizePx = boundingRect().size();
    applyInitialScale();
}

void Tool::setToolSize(const QSizeF& sizePx)
{
    m_targetSizePx = sizePx;
    applyInitialScale();
}

void Tool::applyInitialScale()
{
    const QRectF br = boundingRect();
    if (br.isEmpty())
        return;

    const double sx = m_targetSizePx.width()  / br.width();
    const double sy = m_targetSizePx.height() / br.height();
    m_uniformScale = std::min(sx, sy);

    setScale(m_uniformScale);
    updateOrigin();

    if (scene())
        scene()->update();
}

void Tool::updateOrigin()
{
    setTransformOriginPoint(boundingRect().center());
}

void Tool::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // Solo rotar si está pulsado Shift
    if (!(QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
        event->ignore();
        return;
    }

    const int delta = event->delta(); // Qt5: 120 por "clic" de rueda
    if (delta == 0) {
        event->ignore();
        return;
    }

    // Igual que antes: rotación suave
    double deltaDegrees = (delta / 8.0) * 0.1; // ≈ 1.5° por "clic"
    m_angleDeg += deltaDegrees;
    setRotation(m_angleDeg);

    if (scene())
        scene()->update();

    event->accept();
}

void Tool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    setPlaced(!m_placed);
}

void Tool::setPlaced(bool placed)
{
    m_placed = placed;
    // Keep the item movable; movement will be controlled by event handlers
    setFlag(QGraphicsItem::ItemIsMovable, true);
    // Visual feedback: slightly reduce opacity when placed
    if (m_placed)
        setOpacity(0.85);
    else
        setOpacity(1.0);
}

QPointF Tool::projectPoint(const QPointF& scenePoint, ProjectEdge edge) const
{
    // Local endpoints along the horizontal center line of the bounding rect
    const QRectF br = boundingRect();
    if (br.isEmpty())
        return mapToScene(br.center());

    // Determine which local Y to use: center, top or bottom
    qreal localY = br.center().y();
    if (edge == EdgeTop) localY = br.top();
    else if (edge == EdgeBottom) localY = br.bottom();

    QPointF localA(br.left(), localY);
    QPointF localB(br.right(), localY);

    QPointF a = mapToScene(localA);
    QPointF b = mapToScene(localB);

    // Project point onto segment AB
    const double dx = b.x() - a.x();
    const double dy = b.y() - a.y();
    const double len2 = dx*dx + dy*dy;
    if (len2 <= 1e-8)
        return a;

    const double t = ((scenePoint.x()-a.x())*dx + (scenePoint.y()-a.y())*dy) / len2;
    const double tc = qBound(0.0, t, 1.0);
    return QPointF(a.x() + tc*dx, a.y() + tc*dy);
}

void Tool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    const Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
    if (m_placed && !(mods & Qt::ControlModifier)) {
        // If placed and Ctrl isn't held, prevent movement but allow selection
        setSelected(true);
        event->accept();
        return;
    }

    QGraphicsSvgItem::mousePressEvent(event);
}
