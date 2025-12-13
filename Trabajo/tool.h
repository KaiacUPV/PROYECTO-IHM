#ifndef TOOL_H
#define TOOL_H

#include <QGraphicsSvgItem>
#include <QGraphicsSceneWheelEvent>
class QGraphicsSceneMouseEvent;
#include <QSizeF>

class Tool : public QGraphicsSvgItem
{
public:
    explicit Tool(const QString& svgResourcePath,
                  QGraphicsItem* parent = nullptr);

    // Escala uniforme de la regla a un tamaño objetivo (en píxeles de escena)
    void setToolSize(const QSizeF& sizePx);

    // Mark tool as placed (anchored) in the scene. When placed, it cannot be moved.
    void setPlaced(bool placed);
    bool isPlaced() const { return m_placed; }

    // Project a scene point onto an axis of the tool.
    // Edge specifies whether to project onto the center axis or top/bottom edge.
    enum ProjectEdge { EdgeCenter = 0, EdgeTop = 1, EdgeBottom = 2 };
    QPointF projectPoint(const QPointF& scenePoint, ProjectEdge edge = EdgeCenter) const;

protected:
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void applyInitialScale();
    void updateOrigin();

    QSizeF m_targetSizePx;
    double m_uniformScale = 1.0;
    double m_angleDeg     = 0.0;
    bool m_placed = false;
};

#endif // TOOL_H
