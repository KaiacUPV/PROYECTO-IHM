#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    // Panel usuario / problemas
    void onPerfil();
    void onProblemas();

    // Herramientas
    void onTexto();
    void onPunto();
    void onLinea();
    void onArco();
    void onColor();
    void onMover();
    void onRegla();
    void onZoom();
    void onBorrar();
    void onLimpiar();

private:
    Ui::MainWindow *ui;


    // Mapa
    QGraphicsScene *scene;
    QGraphicsView  *view;
    QPixmap cartaPixmap;

    // Estados de herramientas
    enum ToolType { NONE, TOOL_PUNTO, TOOL_LINEA, TOOL_TEXTO, TOOL_ARCO,
                    TOOL_COLOR, TOOL_MOVER, TOOL_REGLA, TOOL_BORRAR };
    ToolType currentTool = NONE;

    // Para dibujar líneas
    bool drawingLine = false;
    QPointF lineStart;
    QGraphicsLineItem *tempLine = nullptr;

    // Para arco
    int arcStep = 0;
    QPointF arcA, arcB;

    // Color activo
    QColor activeColor = Qt::red;
    int activeWidth = 3;
    int activeFontSize = 16;


    // Zoom
    double zoomLevel = 1.0;
    void applyZoom(double factor);

    // Cargar carta
    void loadCarta();
};

#endif // MAINWINDOW_H
