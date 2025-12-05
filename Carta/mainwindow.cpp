#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QAction>
#include <QMouseEvent> // NUEVO
#include <QKeyEvent> // NUEVO
#include <QPen> // NUEVO
#include <QApplication> // NUEVO

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , view(new QGraphicsView(this))
{
    ui->setupUi(this);

    setWindowTitle("Carta Náutica");
    view->setScene(scene);
    setCentralWidget(view);

    //cargamos la carta nautica en un PixmapItem
    QPixmap pm(":/resources/carta_nautica.jpg");
    QGraphicsPixmapItem *item = scene->addPixmap(pm);
    item->setZValue(0);

    // escalado en mi portatil para que se ajuste aprox a mi ventana
    view->scale(0.20, 0.20);
    m_scaleFactor = 0.20;

    //DragMode permite mover toda la imagen con el ratón
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    // el EventFilter evita el problema de que los eventos
    // lleguen primero al widget y no al QGraphicsView
    view->viewport()->installEventFilter(this);

    // connect con los QAction, se pude hacer desde designer
    QAction *actZoomIn = ui->toolBar->addAction("+");
    QAction *actZoomOut = ui->toolBar->addAction("-");

    connect(actZoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(actZoomOut, &QAction::triggered, this, &MainWindow::zoomOut);

    // NUEVO: QAction toggle para dibujar línea
    m_actDrawLine = ui->toolBar->addAction(tr("Línea"));
    m_actDrawLine->setCheckable(true);
    connect(m_actDrawLine, &QAction::toggled, this, &MainWindow::setDrawLineMode);

    // ===============================
    //  NUEVO: regla dentro de la escena
    // ===============================
    m_protractor = new Tool(":/resources/icons/transportador.svg");
    scene->addItem(m_protractor);

    // Tamaño "lógico" aproximado, como antes
    m_protractor->setToolSize(QSizeF(580, 380));

    // Z alta para que quede en overlay
    m_protractor->setZValue(1000);

    // Posición inicial en la esquina superior izquierda de la vista
    m_protractor->setPos(view->mapToScene(20, 20));
    m_protractor->setPos(QPoint(20, 20));
}

void MainWindow::zoomIn()
{
    applyZoom(1.15);
}

void MainWindow::zoomOut()
{
    applyZoom(1.0 / 1.15);
}

void MainWindow::applyZoom(double factor)
{
    // factor > 1 => acercar; factor < 1 => alejar
    double newScale = m_scaleFactor * factor;
    const double minScale = 0.01;
    const double maxScale = 1;

    if (newScale < minScale) {
        factor = minScale / m_scaleFactor;
        newScale = minScale;
    } else if (newScale > maxScale) {
        factor = maxScale / m_scaleFactor;
        newScale = maxScale;
    }

    view->scale(factor, factor);
    m_scaleFactor = newScale;
}

void MainWindow::setDrawLineMode(bool enabled)
{
    m_drawLineMode = enabled;

    if (m_drawLineMode) {
        view->setCursor(Qt::CrossCursor);
    } else {
        view->unsetCursor();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == view->viewport()) {
        if (!m_drawLineMode)
            return false; // no estamos en modo línea, que lo gestione la vista

        if (event->type() == QEvent::MouseButtonPress) {
            auto *e = static_cast<QMouseEvent*>(event);
            if (e->button() == Qt::RightButton) {
                QPointF scenePos = view->mapToScene(e->pos());
                m_lineStart = scenePos;

                QPen pen(Qt::red, 8);
                m_currentLineItem = new QGraphicsLineItem();
                m_currentLineItem->setZValue(10);
                m_currentLineItem->setPen(pen);
                m_currentLineItem->setLine(QLineF(m_lineStart, m_lineStart));
                scene->addItem(m_currentLineItem);

                return true; // consumimos el evento
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            auto *e = static_cast<QMouseEvent*>(event);
            if (e->buttons() & Qt::RightButton && m_currentLineItem) {
                QPointF p2 = view->mapToScene(e->pos());
                m_currentLineItem->setLine(QLineF(m_lineStart, p2));
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            auto *e = static_cast<QMouseEvent*>(event);
            if (e->button() == Qt::RightButton && m_currentLineItem) {
                // aquí podrías validar la longitud, borrar si es muy corta, etc.
                m_currentLineItem = nullptr;
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(obj, event);
}


MainWindow::~MainWindow()
{
    delete ui;
}
