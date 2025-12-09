#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QtMath>
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QButtonGroup>

#include <QRandomGenerator>
#include <algorithm>
#include <random>

#include "usuario.h" // tu diálogo de login
#include "login.h"
#include "signup.h"

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

    ui->btnZoomIn->setIcon(QIcon(":icon/resources/icons/zoom_in.png"));
    ui->btnZoomIn->setIconSize(iconSize);

    ui->btnZoomOut->setIcon(QIcon(":icon/resources/icons/zoom_out.png"));
    ui->btnZoomOut->setIconSize(iconSize);

    // --- Herramientas especiales ---
    ui->btnRegla->setIcon(QIcon(":icon/resources/icons/regla.png"));
    ui->btnRegla->setIconSize(iconSize);

    ui->btnTransportador->setIcon(QIcon(":icon/resources/icons/transportador.png"));
    ui->btnTransportador->setIconSize(iconSize);

    ui->btnCompas->setIcon(QIcon(":icon/resources/icons/compas.png"));
    ui->btnCompas->setIconSize(iconSize);

    // ----Boton de volver-----
    ui->btnBack1->setIcon(QIcon(":icon/resources/icons/back.png"));
    ui->btnBack1->setIconSize(iconSize);

    ui->btnBack2->setIcon(QIcon(":icon/resources/icons/back.png"));
    ui->btnBack2->setIconSize(iconSize);

    //-----Perfil por defecto-----
    QPixmap pixmap(":icon/resources/icons/perfil.jpg");   // Ruta del recurso o archivo
    ui->lblUserAvatar->setPixmap(pixmap);
    ui->lblUserAvatar->setScaledContents(true);


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
    connect(ui->btnZoomIn,    &QToolButton::clicked, this, &MainWindow::onZoomIn);
    connect(ui->btnZoomOut,    &QToolButton::clicked, this, &MainWindow::onZoomOut);
    connect(ui->btnBorrar,  &QToolButton::clicked, this, &MainWindow::onBorrar);
    connect(ui->btnLimpiar, &QToolButton::clicked, this, &MainWindow::onLimpiar);

    //Boton de ir atrás
    connect(ui->btnBack1, &QToolButton::clicked, this, &MainWindow::back);
    connect(ui->btnBack2, &QToolButton::clicked, this, &MainWindow::back);

    //----Coneciones(usuario) ---
    connect(ui->btnlogin, &QToolButton::clicked, this, &MainWindow::onlogin);

    // --- Conexiones (problemas) ---
    connect(ui->btnAleatorio, &QToolButton::clicked, this, &MainWindow::on_btnAleatorio_clicked);
    connect(ui->btnCorregir,  &QToolButton::clicked, this, &MainWindow::on_btnCorregir_clicked);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_currentIndexChanged);

    // Agrupar radio buttons (opcional pero útil)
    QButtonGroup *grp = new QButtonGroup(this);
    grp->addButton(ui->radioBtnA, 0);
    grp->addButton(ui->radioBtnB, 1);
    grp->addButton(ui->radioBtnC, 2);
    grp->addButton(ui->radioBtnD, 3);

    // Inicializar lista de problemas (si Navigation ya tiene datos)
    initializeProblems();

    // Si hay problemas, cargar el primero por defecto
    if (ui->comboBox->count() > 0)
    {
        ui->comboBox->setCurrentIndex(0);
        loadProblem(0);
    }

    setWindowTitle("Carta Náutica - IHM");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//=========Problemas===============
void MainWindow::initializeProblems()
{
    auto &nav = Navigation::instance();
    const auto &problems = nav.problems();

    ui->comboBox->clear();
    for (int i = 0; i < problems.size(); ++i)
        ui->comboBox->addItem(QString::number(i+1));
}

void MainWindow::loadProblem(int index)
{
    auto &nav = Navigation::instance();
    const auto &problems = nav.problems();

    if (index < 0 || index >= problems.size())
        return;

    m_currentProblem = problems[index];

    // Mostrar enunciado
    ui->enunciado->setPlainText(m_currentProblem.text());

    // Copiar las respuestas y barajarlas
    m_currentAnswersRandom = m_currentProblem.answers();

    // Protegemos en caso de problemas mal formados
    if (m_currentAnswersRandom.size() < 4) {
        QMessageBox::warning(this, "Problema", "El problema seleccionado no tiene 4 respuestas definidas.");
        // limpiar textos
        ui->radioBtnA->setText("A:");
        ui->radioBtnB->setText("B:");
        ui->radioBtnC->setText("C:");
        ui->radioBtnD->setText("D:");
        m_correctAnswerIndex = -1;
        return;
    }

    // Barajar
    std::shuffle(m_currentAnswersRandom.begin(), m_currentAnswersRandom.end(),
                 std::mt19937{static_cast<unsigned int>(std::random_device{}())});

    // Buscar cuál es la correcta después de mezclar
    m_correctAnswerIndex = -1;
    for (int i = 0; i < m_currentAnswersRandom.size(); ++i) {
        if (m_currentAnswersRandom[i].validity()) {
            m_correctAnswerIndex = i;
            break;
        }
    }

    // Mostrar respuestas en los botones (con prefijo para claridad)
    ui->radioBtnA->setText(QStringLiteral("A: ") + m_currentAnswersRandom[0].text());
    ui->radioBtnB->setText(QStringLiteral("B: ") + m_currentAnswersRandom[1].text());
    ui->radioBtnC->setText(QStringLiteral("C: ") + m_currentAnswersRandom[2].text());
    ui->radioBtnD->setText(QStringLiteral("D: ") + m_currentAnswersRandom[3].text());

    // Desmarcar
    ui->radioBtnA->setChecked(false);
    ui->radioBtnB->setChecked(false);
    ui->radioBtnC->setChecked(false);
    ui->radioBtnD->setChecked(false);
}

void MainWindow::on_btnAleatorio_clicked()
{
    auto &nav = Navigation::instance();
    int n = nav.problems().size();
    if (n == 0) return;

    int index = QRandomGenerator::global()->bounded(n);
    ui->comboBox->setCurrentIndex(index);
    loadProblem(index);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    loadProblem(index);
}

void MainWindow::on_btnCorregir_clicked()
{
    int selected = -1;

    if (ui->radioBtnA->isChecked()) selected = 0;
    if (ui->radioBtnB->isChecked()) selected = 1;
    if (ui->radioBtnC->isChecked()) selected = 2;
    if (ui->radioBtnD->isChecked()) selected = 3;

    if (selected == -1)
    {
        QMessageBox::warning(this, "Aviso", "Debes seleccionar una respuesta.");
        return;
    }

    if (m_correctAnswerIndex == -1)
    {
        QMessageBox::warning(this, "Aviso", "Este problema no tiene respuesta correcta definida.");
        return;
    }

    // --- Comprobar ---
    if (selected == m_correctAnswerIndex)
    {
        QMessageBox::information(this, "Correcto", "¡Respuesta correcta!");
        m_sessionHits++;
    }
    else
    {
        QMessageBox::critical(this, "Incorrecto", "La respuesta no es correcta.");
        m_sessionFaults++;
    }
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
    ui->stackedWidget->setCurrentWidget(ui->page_usuario);

    // Expandir el splitter hacia la derecha
    QList<int> sizes;
    sizes << 150 << 400;  // izquierda, derecha (ajusta a tu gusto)
    ui->splitter->setSizes(sizes);
}
void MainWindow::onlogin()
{
    login *dlg = new login();
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    connect(dlg, &login::loginSuccess,
            this, [this](const User &u)
            {
                // Guardar en MainWindow el usuario y el nick actual
                this->m_loggedUser = u;            // declara User m_loggedUser; en MainWindow.h
                this->currentNickName = u.nickName();
                // marca que hay alguien logeado si lo necesitas
                this->m_isLogged = true;

                QMessageBox::information(this, "Bienvenido",
                                         "Bienvenido " + u.nickName());

                ui->stackedWidget->setCurrentWidget(ui->page);

                QList<int> sizes;
                sizes << 150 << 900;
                ui->splitter->setSizes(sizes);
            });


    connect(dlg, &login::openSignup, this, [this]()
            {
                signup *reg = new signup();
                reg->setAttribute(Qt::WA_DeleteOnClose);

                connect(reg, &signup::signupSuccess, this, [this]()
                        {
                            QMessageBox::information(this, "Registro", "Cuenta creada. Ahora inicia sesión.");
                        });

                reg->show();
            });

    dlg->show();
}

void MainWindow::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page);

    // Expandir el splitter hacia la derecha
    QList<int> sizes;
    sizes << 150 << 100;  // izquierda, derecha (ajusta a tu gusto)
    ui->splitter->setSizes(sizes);
}

void MainWindow::onProblemas()
{
    // Cambiar a la página 2 del stackedWidget
    ui->stackedWidget->setCurrentWidget(ui->page_problem);

    // Expandir el splitter hacia la derecha
    QList<int> sizes;
    sizes << 150 << 400;  // izquierda, derecha (ajusta a tu gusto)
    ui->splitter->setSizes(sizes);
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
void MainWindow::onZoomIn()   { zoomLevel += 0.1; view->scale(1.1,1.1); }
void MainWindow::onZoomOut()   { zoomLevel -= 0.1; view->scale(0.9, 0.9); }
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
    // ============================================================
    //          MOVIMIENTO DEL MAPA CON CLICK DERECHO
    // ============================================================
    if (obj == view->viewport())
    {
        // --- Botón derecho presionado ---
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            // CLICK DERECHO → MOVER MAPA
            if (e->button() == Qt::RightButton)
            {
                view->setDragMode(QGraphicsView::ScrollHandDrag);

                // Simulamos un left click para activar el drag de QGraphicsView
                QMouseEvent fakePress(
                    QEvent::MouseButtonPress,
                    e->pos(),
                    Qt::LeftButton,
                    Qt::LeftButton,
                    e->modifiers()
                    );
                QApplication::sendEvent(obj, &fakePress);

                return true;
            }

            // CLICK IZQUIERDO → herramientas
            if (e->button() == Qt::LeftButton)
            {
                QPointF p = view->mapToScene(e->pos());

                // ======================================================
                //                        PUNTO
                // ======================================================
                if (currentTool == TOOL_PUNTO)
                {
                    double r = activeWidth * 1.2;
                    scene->addEllipse(p.x()-r, p.y()-r, 2*r, 2*r,
                                      QPen(activeColor, activeWidth),
                                      QBrush(activeColor));
                    return true;
                }

                // ======================================================
                //                        TEXTO
                // ======================================================
                if (currentTool == TOOL_TEXTO)
                {
                    QString t = QInputDialog::getText(this,"Texto","Introduce texto:");
                    if (!t.isEmpty())
                    {
                        QGraphicsTextItem *txtItem = scene->addText(t);
                        txtItem->setDefaultTextColor(activeColor);

                        QFont f = txtItem->font();
                        f.setPointSize(activeFontSize);
                        txtItem->setFont(f);

                        txtItem->setPos(p);

                    }
                    return true;
                }

                // ======================================================
                //                        LÍNEA
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
                //                        ARCO
                // ======================================================
                if (currentTool == TOOL_ARCO)
                {
                    if (arcStep == 0) { arcA = p; arcStep = 1; }
                    else if (arcStep == 1) { arcB = p; arcStep = 2; }
                    else
                    {
                        QPointF arcC = p;
                        double r = QLineF(arcA, arcC).length();

                        if (r >= 5)
                        {
                            double startAngle = QLineF(arcA, arcB).angle();
                            double endAngle   = QLineF(arcA, arcC).angle();
                            double span = endAngle - startAngle;
                            if (span < 0) span += 360;

                            QRectF rect(arcA.x()-r, arcA.y()-r, 2*r, 2*r);
                            QPainterPath path;
                            path.arcMoveTo(rect, startAngle);
                            path.arcTo(rect, startAngle, span);

                            scene->addPath(path, QPen(activeColor, activeWidth));
                        }

                        arcStep = 0;
                    }
                    return true;
                }

                // ======================================================
                //                        BORRAR
                // ======================================================
                if (currentTool == TOOL_BORRAR)
                {
                    QGraphicsItem *it = scene->itemAt(p, QTransform());

                    if (it && it->type() != QGraphicsPixmapItem::Type)
                        delete it;

                    return true;
                }
            } // fin herramientas click izquierdo
        }

        // ============================================================
        //                ARRASTRAR LÍNEA (mientras se dibuja)
        // ============================================================
        if (event->type() == QEvent::MouseMove && drawingLine)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            QPointF p = view->mapToScene(e->pos());

            if (tempLine)
                tempLine->setLine(QLineF(lineStart, p));

            return true;
        }

        // ============================================================
        //                FINALIZAR LÍNEA
        // ============================================================
        if (event->type() == QEvent::MouseButtonRelease && drawingLine)
        {
            drawingLine = false;
            tempLine = nullptr;
            return true;
        }

        // --- Botón derecho soltado ---
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            if (e->button() == Qt::RightButton)
            {
                // Simular release del botón izquierdo
                QMouseEvent fakeRelease(
                    QEvent::MouseButtonRelease,
                    e->pos(),
                    Qt::LeftButton,
                    Qt::LeftButton,
                    e->modifiers()
                    );
                QApplication::sendEvent(obj, &fakeRelease);

                view->setDragMode(QGraphicsView::NoDrag);
                return true;
            }
        }
    }

    // ============================================================
    //              ZOOM CONTROL + RUEDA
    // ============================================================
    if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *we = static_cast<QWheelEvent*>(event);

        // Solo zoom con CTRL
        if (!(we->modifiers() & Qt::ControlModifier))
            return false;

        if (we->angleDelta().y() > 0)
            applyZoom(1.15);
        else
            applyZoom(1.0 / 1.15);

        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}
