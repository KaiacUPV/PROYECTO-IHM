#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QtMath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    // =========================
    //   ASIGNAR ICONOS PNG
    // =========================

    QSize iconSize(32, 32);

    // --- Herramientas ---
    ui->btnTexto->setIcon(QIcon(":/icon/resources/icons/texto.png"));
    ui->btnTexto->setIconSize(iconSize);

    ui->btnColor->setIcon(QIcon(":icon/resources/icons/color.png"));
    ui->btnColor->setIconSize(iconSize);

    ui->btnArco->setIcon(QIcon(":icon/resources/icons/arco.png"));
    ui->btnArco->setIconSize(iconSize);

    ui->btnLinea->setIcon(QIcon(":icon/resources/icons/linea.png"));
    ui->btnLinea->setIconSize(iconSize);

    ui->btnPunto->setIcon(QIcon(":icon/resources/icons/punto.png"));
    ui->btnPunto->setIconSize(iconSize);

    // --- Configuración ---
    ui->btnLimpiar->setIcon(QIcon(":icon/resources/icons/limpiar.png"));
    ui->btnLimpiar->setIconSize(iconSize);

    ui->btnBorrar->setIcon(QIcon(":icon/resources/icons/borrar.png"));
    ui->btnBorrar->setIconSize(iconSize);

    ui->btnMover->setIcon(QIcon(":icon/resources/icons/mover.png"));
    ui->btnMover->setIconSize(iconSize);

    ui->btnZoom->setIcon(QIcon(":icon/resources/icons/zoom.png"));
    ui->btnZoom->setIconSize(iconSize);

    // --- Herramientas especiales ---
    ui->btnRegla->setIcon(QIcon(":icon/resources/icons/regla.png"));
    ui->btnRegla->setIconSize(iconSize);

    ui->btnTransportador->setIcon(QIcon(":icon/resources/icons/transportador.png"));
    ui->btnTransportador->setIconSize(iconSize);

    ui->btnCompas->setIcon(QIcon(":icon/resources/icons/compas.png"));
    ui->btnCompas->setIconSize(iconSize);

    // === Insertar el QGraphicsView dentro del mapwidget ===
    view = new QGraphicsView(ui->mapwidget);
    view->setScene(scene);
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    QVBoxLayout *lay = new QVBoxLayout(ui->mapwidget);
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(view);

    // Cargar carta náutica
    loadCarta();

    // Recibir eventos del ratón
    view->viewport()->installEventFilter(this);

    // Conexiones panel
    connect(ui->Boton_Usuario, &QPushButton::clicked, this, &MainWindow::onPerfil);
    connect(ui->btnProblemas,  &QToolButton::clicked, this, &MainWindow::onProblemas);

    // Herramientas
    connect(ui->btnTexto,   &QToolButton::clicked, this, &MainWindow::onTexto);
    connect(ui->btnPunto,   &QToolButton::clicked, this, &MainWindow::onPunto);
    connect(ui->btnLinea,   &QToolButton::clicked, this, &MainWindow::onLinea);
    connect(ui->btnArco,    &QToolButton::clicked, this, &MainWindow::onArco);
    connect(ui->btnColor,   &QToolButton::clicked, this, &MainWindow::onColor);


    connect(ui->btnRegla, &QToolButton::clicked, this, &MainWindow::onRegla);
    connect(ui->btnCompas, &QToolButton::clicked, this, &MainWindow::onCompas);
    connect(ui->btnTransportador, &QToolButton::clicked, this, &MainWindow::onTransportador);

    connect(ui->btnMover,   &QToolButton::clicked, this, &MainWindow::onMover);
    connect(ui->btnZoom,    &QToolButton::clicked, this, &MainWindow::onZoom);
    connect(ui->btnBorrar,  &QToolButton::clicked, this, &MainWindow::onBorrar);
    connect(ui->btnLimpiar, &QToolButton::clicked, this, &MainWindow::onLimpiar);


    setWindowTitle("Carta Náutica - IHM");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==========================================================
//     CARGAR CARTA
// ==========================================================
void MainWindow::loadCarta()
{
    scene->clear();

    cartaPixmap = QPixmap(":/resources/carta_nautica.jpg");
    if (cartaPixmap.isNull())
        qDebug() << "ERROR cargando carta_náutica.jpg";

    QGraphicsPixmapItem* item = scene->addPixmap(cartaPixmap);
    item->setScale(0.2); // aumenta un 50%



}

// ==========================================================
//     BOTONES DEL PANEL LATERAL
// ==========================================================

void MainWindow::onPerfil()
{
    QMessageBox::information(this,"Perfil","Ventana perfil aún no implementada.");
}

void MainWindow::onProblemas()
{
    QMessageBox::information(this,"Problemas","Ventana problemas aún no implementada.");
}

// ==========================================================
//     HERRAMIENTAS
// ==========================================================

void MainWindow::onTexto()  { currentTool = TOOL_TEXTO; }
void MainWindow::onPunto()  { currentTool = TOOL_PUNTO; }
void MainWindow::onLinea()  { currentTool = TOOL_LINEA; }
void MainWindow::onArco()   { currentTool = TOOL_ARCO; arcStep = 0; }
void MainWindow::onColor()
{
     // Elegir color
    activeColor = QColorDialog::getColor(Qt::red, this);
    currentTool = TOOL_COLOR;

    // Elegir grosor
    bool ok1;
    int w = QInputDialog::getInt(this, "Grosor de línea",
                                 "Introduce un grosor (1–20):",
                                 activeWidth,
                                 1, 20, 1, &ok1);
    if (ok1) activeWidth = w;

    // Elegir tamaño del texto
    bool ok2;
    int fs = QInputDialog::getInt(this, "Tamaño del texto",
                                  "Introduce tamaño del texto (8–72):",
                                  activeFontSize,
                                  8, 72, 1, &ok2);
    if (ok2) activeFontSize = fs;

    QMessageBox::information(this, "Color",
                             "Color, grosor y tamaño de texto configurados.");
}
void MainWindow::onMover()  { currentTool = TOOL_MOVER; }
void MainWindow::onZoom()   { zoomLevel += 0.1; view->scale(1.1,1.1); }
void MainWindow::onBorrar() { currentTool = TOOL_BORRAR; }
void MainWindow::onLimpiar()
{
    loadCarta();
}
void MainWindow::onRegla()
{
    if (!regla) {
        regla = new Tool(":icon/resources/icons/ruler.svg");
        regla->setToolSize(QSizeF(350, 60));
        scene->addItem(regla);
    }

    centerToolOnView(regla);
}
void MainWindow::onCompas()
{
    if (!compas) {
        compas = new Tool(":icon/resources/icons/compass_leg.svg");
        compas->setToolSize(QSizeF(250, 250));
        scene->addItem(compas);
    }

    centerToolOnView(compas);
}

void MainWindow::onTransportador()
{
    if (!transportador) {
        transportador = new Tool(":icon/resources/icons/transportador.svg");
        transportador->setToolSize(QSizeF(400, 250));
        scene->addItem(transportador);
    }

    centerToolOnView(transportador);
}

void MainWindow::centerToolOnView(Tool *tool)
{
    if (!tool) return;

    QRectF viewRect = view->mapToScene(view->viewport()->rect()).boundingRect();

    QPointF centerPos(
        viewRect.center().x() - tool->boundingRect().width()  / 2,
        viewRect.center().y() - tool->boundingRect().height() / 2
        );

    tool->setPos(centerPos);
}


// ==========================================================
//     ZOOM CON LA RUEDA DEL RATÓN
// ==========================================================

void MainWindow::wheelEvent(QWheelEvent *event)
{
    double factor = (event->angleDelta().y() > 0) ? 1.15 : 0.85;
    view->scale(factor, factor);
    zoomLevel *= factor;
    event->accept();
}

void MainWindow::applyZoom(double factor)
{
    // Control de límites
    double newZoom = zoomLevel * factor;

    const double minZoom = 0.1;
    const double maxZoom = 4.0;

    if (newZoom < minZoom) {
        factor = minZoom / zoomLevel;
        newZoom = minZoom;
    }
    else if (newZoom > maxZoom) {
        factor = maxZoom / zoomLevel;
        newZoom = maxZoom;
    }

    view->scale(factor, factor);
    zoomLevel = newZoom;
}


// ==========================================================
//     EVENTOS DE RATÓN
// ==========================================================

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == view->viewport())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            auto *e = static_cast<QMouseEvent*>(event);
            QPointF p = view->mapToScene(e->pos());

            // ======================================================
            //                  PUNTO
            // ======================================================
            if (currentTool == TOOL_PUNTO)
            {

                    int r = activeWidth * 1.2;
                    scene->addEllipse(p.x()-r, p.y()-r, 2*r, 2*r,
                                  QPen(activeColor, activeWidth),
                                  QBrush(activeColor));

                return true;
            }

            // ======================================================
            //                  TEXTO
            // ======================================================
            if (currentTool == TOOL_TEXTO)
            {
                QString txt = QInputDialog::getText(this,"Texto","Introduce texto:");
                if (!txt.isEmpty())
                {
                    QGraphicsTextItem *txt = scene->addText("Texto");
                    txt->setDefaultTextColor(activeColor);

                    QFont f = txt->font();
                    f.setPointSize(activeFontSize);
                    txt->setFont(f);

                }
                return true;
            }

            // ======================================================
            //                  LÍNEA
            // ======================================================
            if (currentTool == TOOL_LINEA)
            {
                drawingLine = true;
                lineStart = p;

                tempLine = scene->addLine(QLineF(p,p),
                                          QPen(activeColor, activeWidth));
                return true;
            }

            // ======================================================
            //                  ARCOS
            // ======================================================
            if (currentTool == TOOL_ARCO)
            {
                if (arcStep == 0) {
                    arcA = p;
                    arcStep = 1;
                }
                else if (arcStep == 1) {
                    arcB = p;
                    arcStep = 2;
                }
                else {
                    // Tercer punto → define el radio
                    QPointF arcC = p;

                    double r = QLineF(arcA, arcC).length();
                    if (r < 5) { arcStep = 0; return true; }

                    // Cálculo de ángulos
                    double startAngleDeg = QLineF(arcA, arcB).angle();   // ángulo inicial
                    double endAngleDeg   = QLineF(arcA, arcC).angle();   // ángulo final
                    double spanDeg = endAngleDeg - startAngleDeg;

                    // Normalizar
                    if (spanDeg < 0) spanDeg += 360;

                    // Creamos un path con arco
                    QRectF rect(arcA.x() - r, arcA.y() - r, 2*r, 2*r);
                    QPainterPath path;

                    path.arcMoveTo(rect, startAngleDeg);
                    path.arcTo(rect, startAngleDeg, spanDeg);

                    QGraphicsPathItem *arcItem = scene->addPath(path, QPen(activeColor, activeWidth));

                    arcStep = 0;
                    return true;
                }

                return true;
            }

            // ======================================================
            //                  BORRAR
            // ======================================================
            if (currentTool == TOOL_BORRAR)
            {
                QGraphicsItem *item = scene->itemAt(p, QTransform());
                if (item && item->type() != QGraphicsPixmapItem::Type)
                    delete item;
                return true;
            }
        }

        // ======================================================
        //                  ARRASTRAR LÍNEA
        // ======================================================
        if (event->type() == QEvent::MouseMove && drawingLine)
        {
            auto *e = static_cast<QMouseEvent*>(event);
            QPointF p = view->mapToScene(e->pos());
            if (tempLine)
                tempLine->setLine(QLineF(lineStart, p));
            return true;
        }

        // ======================================================
        //                  SUELTA LÍNEA
        // ======================================================
        if (event->type() == QEvent::MouseButtonRelease && drawingLine)
        {
            drawingLine = false;
            tempLine = nullptr;
            return true;
        }
    }

    // =====================
    //     ZOOM CTRL + RUEDA
    // =====================
    if (event->type() == QEvent::Wheel)
    {
        auto *we = static_cast<QWheelEvent*>(event);

        // Si CTRL NO está presionado → no es zoom
        if (!(we->modifiers() & Qt::ControlModifier))
            return false;

        // Delta positivo = rueda hacia arriba = acercar
        if (we->angleDelta().y() > 0)
            applyZoom(1.15);
        else
            applyZoom(1.0 / 1.15);

        return true;   // Consumimos el evento
    }


    return QMainWindow::eventFilter(obj, event);
}
