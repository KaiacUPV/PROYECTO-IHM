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
#include <QShortcut>
#include <limits>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <algorithm>
#include <random>
#include <QFileDialog>
#include <QSpinBox>
#include <QRegularExpression>
#include <QCryptographicHash>

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
    ui->btnTexto->setChecked(true);
    ui->btnTexto->setIconSize(iconSize);

    ui->btnColor->setIcon(QIcon(":icon/resources/icons/color.png"));
    ui->btnColor->setChecked(true);
    ui->btnColor->setIconSize(iconSize);

    ui->btnLinea->setIcon(QIcon(":icon/resources/icons/linea.png"));
    ui->btnLinea->setChecked(true);
    ui->btnLinea->setIconSize(iconSize);

    ui->btnPunto->setIcon(QIcon(":icon/resources/icons/punto.png"));
    ui->btnPunto->setChecked(true);
    ui->btnPunto->setIconSize(iconSize);

    // --- Configuración ---
    ui->btnLimpiar->setIcon(QIcon(":icon/resources/icons/limpiar.png"));
    ui->btnLimpiar->setChecked(true);
    ui->btnLimpiar->setIconSize(iconSize);

    ui->btnBorrar->setIcon(QIcon(":icon/resources/icons/borrar.png"));
    ui->btnBorrar->setChecked(true);
    ui->btnBorrar->setIconSize(iconSize);

    ui->btnMover->setIcon(QIcon(":icon/resources/icons/mover.png"));
    ui->btnMover->setChecked(true);
    ui->btnMover->setIconSize(iconSize);

    ui->btnZoomIn->setIcon(QIcon(":icon/resources/icons/zoom_in.png"));
    ui->btnZoomIn->setChecked(true);
    ui->btnZoomIn->setIconSize(iconSize);

    ui->btnZoomOut->setIcon(QIcon(":icon/resources/icons/zoom_out.png"));
    ui->btnZoomOut->setChecked(true);
    ui->btnZoomOut->setIconSize(iconSize);

    // --- Herramientas especiales ---
    ui->btnRegla->setIcon(QIcon(":icon/resources/icons/regla.png"));
    ui->btnRegla->setChecked(true);
    ui->btnRegla->setIconSize(iconSize);

    ui->btnTransportador->setIcon(QIcon(":icon/resources/icons/transportador.png"));
    ui->btnTransportador->setChecked(true);
    ui->btnTransportador->setIconSize(iconSize);

    ui->btnCompas->setIcon(QIcon(":icon/resources/icons/compas.png"));
    ui->btnCompas->setChecked(true);
    ui->btnCompas->setIconSize(iconSize);

    // --- Botones de volver ---
    ui->btnBack1->setIcon(QIcon(":icon/resources/icons/back.png"));
    ui->btnBack1->setChecked(true);
    ui->btnBack1->setIconSize(iconSize);

    ui->btnBack2->setIcon(QIcon(":icon/resources/icons/back.png"));
    ui->btnBack2->setChecked(true);
    ui->btnBack2->setIconSize(iconSize);

    // === Insertar el QGraphicsView dentro del mapwidget ===
    view = new QGraphicsView(ui->mapwidget);
    view->setScene(scene);
    view->setDragMode(QGraphicsView::NoDrag);  // Desactivar drag por defecto

    // Quitar las barras de desplazamiento
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *lay = new QVBoxLayout(ui->mapwidget);
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(view);

    // Cargar carta náutica
    loadCarta();

    // Mostrar avatar por defecto
    updateUserAvatar();

    // Recibir eventos del ratón
    view->viewport()->installEventFilter(this);

    // =========================
    // CONEXIONES PRINCIPALES
    // =========================

    // Conexiones panel usuario/problemas
    connect(ui->Boton_Usuario, &QPushButton::clicked, this, &MainWindow::onPerfil);
    ui->Boton_Usuario->setChecked(true);

    connect(ui->btnProblemas,  &QToolButton::clicked, this, &MainWindow::onProblemas);
    ui->btnProblemas->setChecked(true);


    // Conexiones perfil (page_usuario)
    connect(ui->Btn_Avatar, &QPushButton::clicked, this, &MainWindow::onEditAvatar);
    ui->Btn_Avatar->setChecked(true);

    connect(ui->btnguardar, &QPushButton::clicked, this, &MainWindow::onSaveProfile);
    ui->btnguardar->setChecked(true);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onCancelProfile);
    ui->pushButton_2->setChecked(true);

    connect(ui->btnBack1, &QToolButton::clicked, this, &MainWindow::back);

    connect(ui->btnShowPassword, &QToolButton::toggled,
            this, &MainWindow::onTogglePassword);

    ui->line_contra->setEchoMode(QLineEdit::Password);
    ui->btnShowPassword->setCheckable(true);
    ui->btnShowPassword->setIcon(QIcon(":icon/resources/icons/eye-closed.svg"));

    // Logout button (page_usuario)
    connect(ui->btnlogin_2, &QPushButton::clicked, this, &MainWindow::onLogout);

    // Botones de herramientas
    connect(ui->btnTexto,   &QToolButton::clicked, this, &MainWindow::onTexto);
    connect(ui->btnPunto,   &QToolButton::clicked, this, &MainWindow::onPunto);
    connect(ui->btnLinea,   &QToolButton::clicked, this, &MainWindow::onLinea);
    connect(ui->btnColor,   &QToolButton::clicked, this, &MainWindow::onColor);

    connect(ui->btnRegla, &QToolButton::clicked, this, &MainWindow::onRegla);
    connect(ui->btnCompas, &QToolButton::clicked, this, &MainWindow::onCompas);
    connect(ui->btnTransportador, &QToolButton::clicked, this, &MainWindow::onTransportador);

    connect(ui->btnMover,   &QToolButton::clicked, this, &MainWindow::onMover);
    connect(ui->btnZoomIn,    &QToolButton::clicked, this, &MainWindow::onZoomIn);
    connect(ui->btnZoomOut,    &QToolButton::clicked, this, &MainWindow::onZoomOut);
    connect(ui->btnBorrar,  &QToolButton::clicked, this, &MainWindow::onBorrar);
    connect(ui->btnLimpiar, &QToolButton::clicked, this, &MainWindow::onLimpiar);

    // Botón login
    connect(ui->btnlogin, &QToolButton::clicked, this, &MainWindow::onlogin);

    // Configurar campos de usuario (readonly username, password masked)
    ui->lblUsuario->setText("Sin Usuario");
    //ui->line_contra->setEchoMode(QLineEdit::Password);

    // Inicializar datos de usuario (por defecto)
    loadProfileUI();

    // --- Small visual polish: stylesheet and avatar-radius ---
    const QString qss = R"(
        QMainWindow { background-color: #F3F6F8; font-family: 'Segoe UI', Arial, sans-serif; }
        QGroupBox { background-color: #FFFFFF; border: 1px solid #E0E6ED; border-radius: 8px; padding: 8px; }
        QToolButton { background: transparent; border-radius: 4px; padding: 2px; border: 2px solid transparent; }
        QToolButton:hover { background-color: rgba(0,0,0,0.05); border: 2px solid rgba(25,118,210,0.15); }
        QToolButton:checked { background-color: rgba(25,118,210,0.2); border: 2px solid #1976D2; color: #1976D2; font-weight: bold; }
        QPushButton { background-color: #1976D2; color: white; padding: 6px 12px; border-radius: 6px; }
        QPushButton:hover { background-color: #155fa0; }
        QPushButton#btnguardar { background-color: #2E7D32; }
        QPushButton#pushButton_2 { background-color: #9E9E9E; }
        QLabel#lblUserAvatar_2 { border-radius: 40px; border: 2px solid #d0d7de; }
        QLabel#lblUserAvatar { border-radius: 64px; border: 2px solid #d0d7de; }
        QLabel#lblUsuario { font-weight: 600; font-size: 14px; }
        QRadioButton::indicator { width: 18px; height: 18px; }
        QGroupBox#groupProblems { background-color: #FFFFFF; }
        QGroupBox#groupTools { background-color: #FFFFFF; }
    )";
    this->setStyleSheet(qss);

    ui->lblUserAvatar_2->setScaledContents(true);
    ui->lblUserAvatar_2->setMinimumSize(80,80);

    // --- Conexiones problemas ---
    connect(ui->btnBack2, &QToolButton::clicked, this, &MainWindow::back);

    // Agrupar radio buttons
    QButtonGroup *grp = new QButtonGroup(this);
    grp->addButton(ui->radioBtnA, 0);
    grp->addButton(ui->radioBtnB, 1);
    grp->addButton(ui->radioBtnC, 2);
    grp->addButton(ui->radioBtnD, 3);

    // Conectar toggled para habilitar/deshabilitar botón corregir
    connect(ui->radioBtnA, &QRadioButton::toggled, this, &MainWindow::onAnswerSelected);
    connect(ui->radioBtnB, &QRadioButton::toggled, this, &MainWindow::onAnswerSelected);
    connect(ui->radioBtnC, &QRadioButton::toggled, this, &MainWindow::onAnswerSelected);
    connect(ui->radioBtnD, &QRadioButton::toggled, this, &MainWindow::onAnswerSelected);
    // Estado inicial
    ui->btnCorregir->setEnabled(false);

    // Inicializar lista de problemas
    initializeProblems();

    // Cargar primer problema por defecto
    if (ui->comboBox->count() > 0)
    {
        ui->comboBox->setCurrentIndex(0);
        loadProblem(0);
    }

    setWindowTitle("Carta Náutica - IHM");

    // Make drawing tool buttons checkable and group them for exclusive selection
    QButtonGroup *toolButtons = new QButtonGroup(this);
    toolButtons->setExclusive(true);
    ui->btnTexto->setCheckable(true);
    ui->btnPunto->setCheckable(true);
    ui->btnLinea->setCheckable(true);
    ui->btnColor->setCheckable(true);
    ui->btnMover->setCheckable(true);
    ui->btnBorrar->setCheckable(true);
    ui->btnRegla->setCheckable(true);
    ui->btnCompas->setCheckable(true);
    ui->btnTransportador->setCheckable(true);
    toolButtons->addButton(ui->btnTexto);
    toolButtons->addButton(ui->btnPunto);
    toolButtons->addButton(ui->btnLinea);
    toolButtons->addButton(ui->btnColor);
    toolButtons->addButton(ui->btnMover);
    toolButtons->addButton(ui->btnBorrar);
    toolButtons->addButton(ui->btnRegla);
    toolButtons->addButton(ui->btnCompas);
    toolButtons->addButton(ui->btnTransportador);

    // Theme toggle shortcut
    QShortcut *themeShortcut = new QShortcut(QKeySequence("Ctrl+T"), this);
    connect(themeShortcut, &QShortcut::activated, this, &MainWindow::toggleTheme);
}

QPixmap MainWindow::makeRoundedPixmap(const QPixmap &src, int diameter)
{
    if (src.isNull()) return QPixmap();
    QPixmap pm = src.scaled(diameter, diameter, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap rounded(diameter, diameter);
    rounded.fill(Qt::transparent);
    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, diameter, diameter);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pm);
    return rounded;
}

void MainWindow::setCircularLabel(QLabel *label, const QPixmap &pixmap, int diameter)
{
    if (!label) return;
    label->setFixedSize(diameter, diameter);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
    label->setMask(QRegion(0, 0, diameter, diameter, QRegion::Ellipse));
}

MainWindow::~MainWindow()
{
    // Persist pending session if the user is still logged in
    if (m_isLogged) {
        try {
            auto &nav = Navigation::instance();
            Session s(m_currentSession.timeStamp(), m_sessionHits, m_sessionFaults);
            nav.addSession(currentNickName, s);
            nav.dao().addSession(currentNickName, s);
        } catch (...) {
            // swallow errors in destructor; nothing to do
        }
    }

    delete ui;
}

// ==========================================================
//     EDICIÓN DE PERFIL
// ==========================================================

void MainWindow::loadProfileUI()
{
    if (m_isLogged) {
        ui->lblUsuario->setText(m_loggedUser.nickName());
        ui->line_email->setText(m_loggedUser.email());

        // Nunca mostrar contraseña
        ui->line_contra->clear();
        ui->line_contra->setPlaceholderText("Introduce nueva contraseña");
        ui->line_contra->setEchoMode(QLineEdit::Password);

        ui->dateUser->setDate(m_loggedUser.birthdate());

        QPixmap pixmap = QPixmap::fromImage(m_loggedUser.avatar());
        setCircularLabel(ui->lblUserAvatar_2,
                         makeRoundedPixmap(pixmap, 80), 80);

        currentAvatarPath = "";
    } else {
        ui->lblUsuario->setText("Sin Usuario");
        ui->line_email->clear();
        ui->line_contra->clear();
        ui->dateUser->setDate(QDate::currentDate());

        ui->lblUserAvatar_2->setPixmap(
            makeRoundedPixmap(QPixmap(":/icon/resources/icons/perfil.jpg"), 80)
            );

        ui->line_contra->setEchoMode(QLineEdit::Password);
        currentAvatarPath = "";
    }
}


void MainWindow::onEditAvatar()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Seleccionar Avatar",
        QString(),
        "Imágenes (*.png *.jpg *.jpeg *.bmp *.gif);;Todos (*.*)"
        );

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);

        if (pixmap.isNull()) {
            QMessageBox::warning(this, "Error", "No se pudo cargar la imagen seleccionada.");
            return;
        }

        currentAvatarPath = filePath;
        setCircularLabel(ui->lblUserAvatar_2, makeRoundedPixmap(pixmap, 80), 80);
        QMessageBox::information(this, "Éxito", "Avatar seleccionado. Guarda los cambios para aplicarlo.");
    }
}

bool MainWindow::validateEmail(const QString &email)
{
    QRegularExpression emailRegex(
        "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"
        );
    return emailRegex.match(email).hasMatch();
}

bool MainWindow::validatePassword(const QString &password)
{
    // Requisitos: mín 8 caracteres, mayúsculas, minúsculas, dígitos
    if (password.length() < 8) {
        QMessageBox::warning(this, "Error", "La contraseña debe tener mínimo 8 caracteres.");
        return false;
    }
    if (!password.contains(QRegularExpression("[A-Z]"))) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos una mayúscula.");
        return false;
    }
    if (!password.contains(QRegularExpression("[a-z]"))) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos una minúscula.");
        return false;
    }
    if (!password.contains(QRegularExpression("[0-9]"))) {
        QMessageBox::warning(this, "Error", "La contraseña debe contener al menos un dígito.");
        return false;
    }
    return true;
}
static QString hashPassword(const QString &password)
{
    return QCryptographicHash::hash(
               password.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}


void MainWindow::onSaveProfile()
{
    if (!m_isLogged) {
        QMessageBox::warning(this, "Error", "Debes estar logueado para editar tu perfil.");
        return;
    }

    // Validar campos
    QString email = ui->line_email->text().trimmed();
    QString newPassword = ui->line_contra->text();   // NUEVA contraseña (opcional)
    QDate birthDate = ui->dateUser->date();

    // Validar email
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error", "El correo electrónico no puede estar vacío.");
        return;
    }
    if (!validateEmail(email)) {
        QMessageBox::warning(this, "Error",
                             "El correo electrónico no es válido.\nFormato: usuario@dominio.com");
        return;
    }

    // 🔐 Validar contraseña SOLO si se quiere cambiar
    QString finalPasswordHash = m_loggedUser.password(); // por defecto, la actual

    if (!newPassword.isEmpty()) {
        if (!validatePassword(newPassword)) {
            return;
        }
        finalPasswordHash = hashPassword(newPassword);
    }

    // Validar fecha de nacimiento
    if (birthDate > QDate::currentDate()) {
        QMessageBox::warning(this, "Error", "La fecha de nacimiento no puede ser futura.");
        return;
    }

    int age = QDate::currentDate().year() - birthDate.year();
    if (age < 13) {
        QMessageBox::warning(this, "Error",
                             "Debes tener al menos 13 años para usar este servicio.");
        return;
    }

    // Avatar
    QImage newAvatar;
    if (!currentAvatarPath.isEmpty()) {
        newAvatar.load(currentAvatarPath);
        if (newAvatar.isNull()) {
            QMessageBox::warning(this, "Error",
                                 "No se pudo cargar la imagen del avatar.");
            return;
        }
    } else {
        newAvatar = m_loggedUser.avatar();
    }

    // ========================================
    // GUARDAR EN BD (navigationdao)
    // ========================================
    try {
        auto &nav = Navigation::instance();

        User updatedUser(
            m_loggedUser.nickName(),  // nickName (no modificable)
            email,                    // email
            finalPasswordHash,        // hash (nuevo o anterior)
            newAvatar,                // avatar
            birthDate                 // birthdate
            );

        // Actualizar en memoria
        m_loggedUser = updatedUser;
        nav.updateUser(updatedUser);
        nav.dao().updateUser(updatedUser);

        QMessageBox::information(this, "Éxito",
                                 "Perfil actualizado correctamente.");

        // Limpiar campo contraseña (buena práctica)
        ui->line_contra->clear();

        loadProfileUI();
        updateUserAvatar();

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error",
                              QString("No se pudo guardar el perfil: ")
                                  + QString::fromStdString(e.what()));
    }
}


void MainWindow::onCancelProfile()
{
    // Recargar datos originales
    loadProfileUI();

    // Volver a la página principal
    back();
}

// ==========================================================
//     PROBLEMAS
// ==========================================================

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

    // Debug: mostrar si hay alguna respuesta marcada en el original
    const auto &orig = m_currentProblem.answers();
    int origCorrectIdx = -1;
    for (int i = 0; i < orig.size(); ++i)
        qDebug() << "Orig answer" << i << ":" << orig[i].text() << "valid:" << orig[i].validity();

    // Protegemos en caso de problemas mal formados
    if (m_currentAnswersRandom.size() < 4) {
        ui->radioBtnA->setText("A:");
        ui->radioBtnB->setText("B:");
        ui->radioBtnC->setText("C:");
        ui->radioBtnD->setText("D:");
        m_correctAnswerIndex = -1;
        ui->btnCorregir->setEnabled(false);
        return;
    }

    // Encontrar índice correcto en el original (si existe)
    for (int i = 0; i < orig.size(); ++i) {
        if (orig[i].validity()) { origCorrectIdx = i; break; }
    }

    // Barajar
    std::shuffle(m_currentAnswersRandom.begin(), m_currentAnswersRandom.end(),
                 std::mt19937{static_cast<unsigned int>(std::random_device{}())});

    // Debug: mostrar respuestas barajadas
    for (int i = 0; i < m_currentAnswersRandom.size(); ++i)
        qDebug() << "Shuffled answer" << i << ":" << m_currentAnswersRandom[i].text() << "valid:" << m_currentAnswersRandom[i].validity();

    // Buscar cuál es la correcta después de mezclar
    m_correctAnswerIndex = -1;
    for (int i = 0; i < m_currentAnswersRandom.size(); ++i) {
        if (m_currentAnswersRandom[i].validity()) { m_correctAnswerIndex = i; break; }
    }

    // Si no la encontramos por validity -> intentar mapear por texto desde original
    if (m_correctAnswerIndex == -1 && origCorrectIdx != -1) {
        const QString origCorrectText = orig[origCorrectIdx].text();
        for (int i = 0; i < m_currentAnswersRandom.size(); ++i) {
            if (m_currentAnswersRandom[i].text() == origCorrectText) {
                m_correctAnswerIndex = i;
                qDebug() << "Mapped correct answer by text to index" << i;
                break;
            }
        }
    }

    // Si aún no hay respuesta correcta, avisar al cargar y evitar que Corregir esté activo
    if (m_correctAnswerIndex == -1) {
        qDebug() << "AVISO: Problema sin respuesta correcta definida (index=" << index << ")";
        ui->btnCorregir->setEnabled(false);
        // opción: informar visualmente al usuario en la enunciado
        ui->enunciado->append("\n\n[AVISO: problema sin respuesta correcta definida]");
    } else {
        ui->btnCorregir->setEnabled(false); // deshabilitado hasta que el usuario seleccione
    }

    // Mostrar respuestas en los botones
    ui->radioBtnA->setText(QStringLiteral("A: ") + m_currentAnswersRandom[0].text());
    ui->radioBtnB->setText(QStringLiteral("B: ") + m_currentAnswersRandom[1].text());
    ui->radioBtnC->setText(QStringLiteral("C: ") + m_currentAnswersRandom[2].text());
    ui->radioBtnD->setText(QStringLiteral("D: ") + m_currentAnswersRandom[3].text());

    // Desmarcar
    ui->radioBtnA->setChecked(false);
    ui->radioBtnB->setChecked(false);
    ui->radioBtnC->setChecked(false);
    ui->radioBtnD->setChecked(false);

    // Forzar estado inicial deshabilitado hasta que el usuario elija una opción
    ui->btnCorregir->setEnabled(false);
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
    else if (ui->radioBtnB->isChecked()) selected = 1;
    else if (ui->radioBtnC->isChecked()) selected = 2;
    else if (ui->radioBtnD->isChecked()) selected = 3;

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

    // Comprobar respuesta
    if (selected == m_correctAnswerIndex)
    {
        QMessageBox::information(this, "Correcto", "¡Respuesta correcta!");
        m_sessionHits++;
        m_currentSession = Session(m_currentSession.timeStamp(), m_sessionHits, m_sessionFaults);
    }
    else
    {
        QMessageBox::critical(this, "Incorrecto", "La respuesta no es correcta.");
        m_sessionFaults++;
        m_currentSession = Session(m_currentSession.timeStamp(), m_sessionHits, m_sessionFaults);
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
    item->setScale(0.2);
}

// ==========================================================
//     BOTONES DEL PANEL LATERAL
// ==========================================================

void MainWindow::onPerfil()
{
    if (!m_isLogged) {
        QMessageBox::information(this, "Información", "Debes iniciar sesión para acceder a tu perfil.");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->page_usuario);
    loadProfileUI();

    QList<int> sizes;
    sizes << 150 << 400;
    ui->splitter->setSizes(sizes);
}

void MainWindow::updateUserAvatar()
{
    if (m_isLogged) {
        QPixmap pixmap = QPixmap::fromImage(m_loggedUser.avatar());
        QPixmap roundedMain = makeRoundedPixmap(pixmap, 128);
        QPixmap roundedSmall = makeRoundedPixmap(pixmap, 80);
        setCircularLabel(ui->lblUserAvatar, roundedMain, 128);
        setCircularLabel(ui->lblUserAvatar_2, roundedSmall, 80);
    } else {
        QPixmap pixmap(":/icon/resources/icons/perfil.jpg");
        setCircularLabel(ui->lblUserAvatar, makeRoundedPixmap(pixmap, 128), 128);
        setCircularLabel(ui->lblUserAvatar_2, makeRoundedPixmap(pixmap, 80), 80);
    }
    ui->lblUserAvatar_2->setScaledContents(true);
    ui->lblUserAvatar->setScaledContents(true);
}

void MainWindow::onlogin()
{
    login *dlg = new login();
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    connect(dlg, &login::loginSuccess, this, &MainWindow::onLoginSuccess);

    connect(dlg, &login::openSignup, this, [this]()
            {
                signup *reg = new signup();
                reg->setAttribute(Qt::WA_DeleteOnClose);

                connect(reg, &signup::signupSuccess, this, &MainWindow::onLoginSuccess);

                reg->show();
            });

    dlg->show();
}

void MainWindow::onLoginSuccess(const User &u)
{
    this->m_loggedUser = u;
    this->currentNickName = u.nickName();
    this->m_isLogged = true;

    // Reiniciar datos de sesión
    m_sessionHits = 0;
    m_sessionFaults = 0;
    m_currentSession = Session(QDateTime::currentDateTime(), 0, 0);

    updateUserAvatar();

    // Cargar sesiones del usuario desde la BD y actualizar memoria
    try {
        auto &nav = Navigation::instance();
        QVector<Session> sessions = nav.dao().loadSessionsFor(u.nickName());
        m_loggedUser.setSessions(sessions);
        nav.updateUser(m_loggedUser);
    } catch (...) {
        // ignore DB errors here — UI still loads
    }

    // Mostrar contraseña y cargar UI del perfil
    loadProfileUI();


    ui->stackedWidget->setCurrentWidget(ui->page);

    QList<int> sizes;
    sizes << 150 << 900;
    ui->splitter->setSizes(sizes);
}

// NEW: logout implementation
void MainWindow::onLogout()
{
    if (!m_isLogged) {
        QMessageBox::information(this, "Información", "No hay sesión iniciada.");
        return;
    }

    // Confirmación
    auto res = QMessageBox::question(this, "Cerrar sesión",
                "¿Deseas cerrar sesión y guardar los cambios en la base de datos?",
                QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes)
        return;

    // 1) Intentar guardar el perfil actual (si hay cambios)
    // onSaveProfile() valida y actualiza m_loggedUser y la BD vía Navigation.
    onSaveProfile();

    // 2) Registrar la sesión y persistir el registro
    {
        try {
            auto &nav = Navigation::instance();
            Session s(QDateTime::currentDateTime(), m_sessionHits, m_sessionFaults);
            nav.addSession(currentNickName, s);
            nav.dao().addSession(currentNickName, s);
        } catch (const std::exception &ex) {
            QMessageBox::warning(this, "Advertencia", QString("No se pudo guardar la sesión: ") + QString::fromStdString(ex.what()));
        }
    }

    // 3) Reset UI y estado
    m_sessionHits = 0;
    m_sessionFaults = 0;
    m_currentSession = Session(QDateTime::currentDateTime(), 0, 0);

    m_loggedUser = User();    // usuario vacío por defecto
    currentNickName.clear();
    m_isLogged = false;
    currentAvatarPath.clear();

    // Restaurar avatar por defecto y enmascarar contraseña
    updateUserAvatar();
    loadProfileUI();

    // Volver a la página inicial
    ui->stackedWidget->setCurrentWidget(ui->page);
    QList<int> sizes; sizes << 150 << 100; ui->splitter->setSizes(sizes);

    //QMessageBox::information(this, "Sesión cerrada", "Sesión cerrada y cambios guardados.");
}

void MainWindow::back()
{
    if (m_isLogged)
    {
        {
            try {
                auto &nav = Navigation::instance();
                Session s(QDateTime::currentDateTime(), m_sessionHits, m_sessionFaults);
                nav.addSession(currentNickName, s);
                nav.dao().addSession(currentNickName, s);
            } catch (const std::exception &ex) {
                QMessageBox::warning(this, "Advertencia", QString("No se pudo guardar la sesión: ") + QString::fromStdString(ex.what()));
            }
        }

        m_sessionHits = 0;
        m_sessionFaults = 0;
    }

    ui->stackedWidget->setCurrentWidget(ui->page);

    QList<int> sizes;
    sizes << 150 << 100;
    ui->splitter->setSizes(sizes);
}

void MainWindow::onProblemas()
{
    if (!m_isLogged) {
        QMessageBox::information(this, "Información", "Debes iniciar sesión para acceder a los problemas.");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->page_problem);

    QList<int> sizes;
    sizes << 150 << 400;
    ui->splitter->setSizes(sizes);
}

// ==========================================================
//     HERRAMIENTAS
// ==========================================================

void MainWindow::onTexto()  { currentTool = TOOL_TEXTO; updateToolSelection(TOOL_TEXTO); }
void MainWindow::onPunto()  { currentTool = TOOL_PUNTO; updateToolSelection(TOOL_PUNTO); }
void MainWindow::onLinea()  { currentTool = TOOL_LINEA; updateToolSelection(TOOL_LINEA); }

void MainWindow::onColor()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Configurar Color, Grosor y Tamaño de Texto");
    dialog.setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Color selection
    QHBoxLayout *colorLayout = new QHBoxLayout();
    QLabel *colorLabel = new QLabel("Color:");
    QPushButton *colorButton = new QPushButton("Seleccionar Color");
    colorButton->setStyleSheet(QString("background-color: %1").arg(activeColor.name()));
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorButton);
    layout->addLayout(colorLayout);

    // Width selection
    QHBoxLayout *widthLayout = new QHBoxLayout();
    QLabel *widthLabel = new QLabel("Grosor de línea:");
    QSpinBox *widthSpin = new QSpinBox();
    widthSpin->setRange(1, 20);
    widthSpin->setValue(activeWidth);
    widthLayout->addWidget(widthLabel);
    widthLayout->addWidget(widthSpin);
    layout->addLayout(widthLayout);

    // Font size selection
    QHBoxLayout *fontLayout = new QHBoxLayout();
    QLabel *fontLabel = new QLabel("Tamaño del texto:");
    QSpinBox *fontSpin = new QSpinBox();
    fontSpin->setRange(8, 72);
    fontSpin->setValue(activeFontSize);
    fontLayout->addWidget(fontLabel);
    fontLayout->addWidget(fontSpin);
    layout->addLayout(fontLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Aceptar");
    QPushButton *cancelButton = new QPushButton("Cancelar");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect color button
    connect(colorButton, &QPushButton::clicked, [&]() {
        QColor newColor = QColorDialog::getColor(activeColor, &dialog);
        if (newColor.isValid()) {
            activeColor = newColor;
            colorButton->setStyleSheet(QString("background-color: %1").arg(activeColor.name()));
        }
    });

    // Connect buttons
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        activeWidth = widthSpin->value();
        activeFontSize = fontSpin->value();
    }

    currentTool = TOOL_COLOR;
    updateToolSelection(TOOL_COLOR);
}

void MainWindow::onMover()  {
    currentTool = TOOL_MOVER;
    updateToolSelection(TOOL_MOVER);
    view->setDragMode(QGraphicsView::ScrollHandDrag);  // Activar drag solo con herramienta mover
}
void MainWindow::onZoomIn()   { zoomLevel += 0.1; view->scale(1.1, 1.1); }
void MainWindow::onZoomOut()  { zoomLevel -= 0.1; view->scale(0.9, 0.9); }
void MainWindow::onBorrar() { currentTool = TOOL_BORRAR; updateToolSelection(TOOL_BORRAR); }

void MainWindow::onLimpiar()
{
    loadCarta();
    clearArcPreview();
}

void MainWindow::onRegla()
{
    if (!regla) {
        regla = new Tool(":icon/resources/icons/ruler.svg");
        regla->setToolSize(QSizeF(350, 60));
        scene->addItem(regla);
    }

    // Toggle placement on repeated clicks: if already placed, unplace for repositioning
    clearArcPreview();
    if (regla->isPlaced())
    {
        regla->setPlaced(false);
        statusBar()->showMessage("Regla desbloqueada: arrastra para mover", 4000);
    }
    else
    {
        centerToolOnView(regla);
        regla->setPlaced(true);
        statusBar()->showMessage("Regla colocada: Mantén Ctrl y arrastra para moverla", 5000);
    }
    currentTool = TOOL_REGLA;
    updateToolSelection(TOOL_REGLA);
}

void MainWindow::onCompas()
{
    currentTool = TOOL_COMPAS;
    arcStep = 0;
    updateToolSelection(TOOL_COMPAS);
    clearArcPreview();

}

void MainWindow::onTransportador()
{
    if (!transportador) {
        transportador = new Tool(":icon/resources/icons/transportador.svg");
        transportador->setToolSize(QSizeF(400, 250));
        scene->addItem(transportador);
    }

    clearArcPreview();
    if (transportador->isPlaced()) { transportador->setPlaced(false); statusBar()->showMessage("Transportador desbloqueado: arrastra para mover", 4000); }
    else { centerToolOnView(transportador); transportador->setPlaced(true); statusBar()->showMessage("Transportador colocado: Mantén Ctrl y arrastra para moverlo", 4000); }
    currentTool = TOOL_TRANSPORTADOR;
    updateToolSelection(TOOL_TRANSPORTADOR);
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

void MainWindow::updateToolSelection(int tool)
{
    // Desactivar drag mode por defecto (solo activo con herramienta mover)
    view->setDragMode(QGraphicsView::NoDrag);

    // Limpiar previews temporales antes de cambiar de herramienta
    clearArcPreview();
    if (tempLine) {
        scene->removeItem(tempLine);
        delete tempLine;
        tempLine = nullptr;
    }
    drawingLine = false;

    // Desmarcar todos los botones de herramientas
    ui->btnTexto->setChecked(false);
    ui->btnPunto->setChecked(false);
    ui->btnLinea->setChecked(false);
    ui->btnColor->setChecked(false);
    ui->btnMover->setChecked(false);
    ui->btnBorrar->setChecked(false);
    ui->btnRegla->setChecked(false);
    ui->btnCompas->setChecked(false);
    ui->btnTransportador->setChecked(false);
    
    // Marcar el botón correspondiente a la herramienta seleccionada
    switch(tool) {
        case TOOL_TEXTO:
            ui->btnTexto->setChecked(true);
            break;
        case TOOL_PUNTO:
            ui->btnPunto->setChecked(true);
            break;
        case TOOL_LINEA:
            ui->btnLinea->setChecked(true);
            break;
        case TOOL_COLOR:
            ui->btnColor->setChecked(true);
            break;
        case TOOL_MOVER:
            ui->btnMover->setChecked(true);
            break;
        case TOOL_BORRAR:
            ui->btnBorrar->setChecked(true);
            break;
        case TOOL_REGLA:
            ui->btnRegla->setChecked(true);
            break;
        case TOOL_COMPAS:
            ui->btnCompas->setChecked(true);
            break;
        case TOOL_TRANSPORTADOR:
            ui->btnTransportador->setChecked(true);
            break;
        default:
            break;
    }
}

// ==========================================================
//     ZOOM CON LA RUEDA DEL RATÓN
// ==========================================================

void MainWindow::wheelEvent(QWheelEvent *event)
{
    // Deshabilitar completamente el zoom con la rueda del mouse
    // Solo se permite zoom con los botones dedicados
    event->ignore();
}

// ==========================================================
//     ATAJOS DE TECLADO PARA HERRAMIENTAS
// ==========================================================

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Atajos sin modificadores
    switch (event->key()) {
        case Qt::Key_R:
            onRegla();
            break;
        case Qt::Key_C:
            onCompas();
            break;
        case Qt::Key_L:
            onLinea();
            break;
        case Qt::Key_P:
            onPunto();
            break;
        case Qt::Key_T:
            onTexto();
            break;
        case Qt::Key_B:
            onBorrar();
            break;
        case Qt::Key_Delete:
            onLimpiar();
            break;
        default:
            break;
    }

    // Atajos con Shift
    if (event->modifiers() & Qt::ShiftModifier) {
        switch (event->key()) {
            case Qt::Key_T:
                onTransportador();
                break;
            case Qt::Key_C:
                onColor();
                break;
            default:
                break;
        }
    }

    // Llamar al handler base para otros eventos
    QMainWindow::keyPressEvent(event);
}

void MainWindow::applyZoom(double factor)
{
    double newZoom = zoomLevel * factor;

    const double minZoom = 1.0;
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

Tool *MainWindow::findNearestPlacedTool(const QPointF &scenePoint)
{
    Tool *best = nullptr;
    double bestDist = std::numeric_limits<double>::infinity();

    auto checkTool = [&](Tool *tool) {
        if (!tool || !tool->isPlaced()) return;
        QPointF proj = tool->projectPoint(scenePoint, Tool::EdgeTop);
        double d = QLineF(scenePoint, proj).length();
        if (d < bestDist) { bestDist = d; best = tool; }
    };

    checkTool(regla);
    checkTool(compas);
    checkTool(transportador);

    return best;
}

void MainWindow::toggleTheme()
{
    m_darkTheme = !m_darkTheme;
    if (m_darkTheme) {
        const QString dark = R"(
            QMainWindow { background-color: #121212; color: #E0E0E0; }
            QGroupBox { background-color: #1E1E1E; border: 1px solid #333; }
            QPushButton { background-color: #BB86FC; color: #000000; }
            QToolButton { background-color: transparent; }
        )";
        this->setStyleSheet(dark);
        statusBar()->showMessage("Tema oscuro activado", 2000);
    } else {
        // Reapply the previous light theme
        const QString light = R"(
            QMainWindow { background-color: #F3F6F8; color: #222; }
            QGroupBox { background-color: #FFFFFF; border: 1px solid #E0E6ED; border-radius: 8px; }
        )";
        this->setStyleSheet(light);
        statusBar()->showMessage("Tema claro activado", 2000);
    }
}

// ==========================================================
//     EVENTOS DE RATÓN
// ==========================================================

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (!scene || !view || obj != view->viewport())
        return QMainWindow::eventFilter(obj, event);
    {
        // --- Wheel event: handle zoom or rotation ---
        if (event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            if (wheelEvent->modifiers() & Qt::ShiftModifier)
            {
                // Shift + wheel: rotate tool if regla or transportador is selected
                if (currentTool == TOOL_REGLA && regla)
                {
                    double delta = wheelEvent->angleDelta().y() > 0 ? 5.0 : -5.0;
                    regla->setRotation(regla->rotation() + delta);
                }
                else if (currentTool == TOOL_TRANSPORTADOR && transportador)
                {
                    double delta = wheelEvent->angleDelta().y() > 0 ? 5.0 : -5.0;
                    transportador->setRotation(transportador->rotation() + delta);
                }
            }
            else
            {
                // Normal wheel: zoom
                if (wheelEvent->angleDelta().y() > 0)
                {
                    onZoomIn();
                }
                else if (wheelEvent->angleDelta().y() < 0)
                {
                    onZoomOut();
                }
            }
            return true;
        }

        // --- Botón presionado ---
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);

            // CLICK CENTRAL → ACTIVAR/DESACTIVAR HERRAMIENTA MOVER
            if (e->button() == Qt::MiddleButton)
            {
                onMover();
                return true;
            }

            // CLICK DERECHO → Ya no activa drag mode (solo con herramienta mover)
            // if (e->button() == Qt::RightButton)
            // {
            //     clearArcPreview();
            //     view->setDragMode(QGraphicsView::ScrollHandDrag);
            //     ...
            // }

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
                    QString t = QInputDialog::getText(this, "Texto", "Introduce texto:");
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
                    Tool *t = findNearestPlacedTool(p);
                    if (t)
                        lineStart = t->projectPoint(p, Tool::EdgeTop);
                    else
                        lineStart = p;

                    tempLine = scene->addLine(QLineF(p, p),
                                              QPen(activeColor, activeWidth));

                    return true;
                }

                // ======================================================
                //                    COMPÁS (ARCO)
                // ======================================================
                if (currentTool == TOOL_COMPAS)
                {
                    // Verificación de seguridad: asegurar que no hay operaciones pendientes
                    if (arcStep < 0 || arcStep > 2) {
                        arcStep = 0;
                        clearArcPreview();
                        return true;
                    }
                    if (arcStep == 0) {
                        arcA = p; arcStep = 1;
                        // Create center marker
                        if (!arcMarkerCenter) {
                            arcMarkerCenter = scene->addEllipse(arcA.x()-3, arcA.y()-3, 6, 6, QPen(Qt::darkGreen), QBrush(Qt::darkGreen));
                            statusBar()->showMessage("Centro del arco fijado", 2500);
                        } else {
                            arcMarkerCenter->setRect(arcA.x()-3, arcA.y()-3, 6, 6);
                        }
                        statusBar()->showMessage("Punto inicio fijado; mueve el ratón y haz clic para finalizar el arco", 4000);
                    }
                    else if (arcStep == 1) {
                        arcB = p; arcStep = 2;
                        // Create start marker
                        if (!arcMarkerStart)
                            arcMarkerStart = scene->addEllipse(arcB.x()-3, arcB.y()-3, 6, 6, QPen(Qt::NoPen), QBrush(Qt::blue));
                        else
                            arcMarkerStart->setRect(arcB.x()-3, arcB.y()-3, 6, 6);
                        // Prepare preview arc with radius from arcA to arcB
                        double r = QLineF(arcA, arcB).length();
                        QRectF rect(arcA.x()-r, arcA.y()-r, 2*r, 2*r);
                        QPainterPath path;
                        double startAngle = QLineF(arcA, arcB).angle();
                        path.arcMoveTo(rect, startAngle);
                        // initialize with zero span
                        if (!tempArc) {
                            QPen previewPen(activeColor, activeWidth);
                            previewPen.setStyle(Qt::DashLine);
                            tempArc = scene->addPath(path, previewPen);
                        } else {
                            tempArc->setPath(path);
                        }
                    }
                    else
                    {
                        QPointF arcC = p;
                        double r = QLineF(arcA, arcB).length();

                        if (r >= 5)
                        {
                            double startAngle = QLineF(arcA, arcB).angle();
                            double endAngle   = QLineF(arcA, arcC).angle();
                            double span = endAngle - startAngle;

                            // Normalizar el span para que esté entre -180 y 180 grados
                            // Esto permite arcos en ambos sentidos: positivo = horario, negativo = antihorario
                            while (span > 180) span -= 360;
                            while (span < -180) span += 360;

                            QRectF rect(arcA.x()-r, arcA.y()-r, 2*r, 2*r);
                            QPainterPath path;
                            path.arcMoveTo(rect, startAngle);
                            path.arcTo(rect, startAngle, span);

                            scene->addPath(path, QPen(activeColor, activeWidth));
                            statusBar()->showMessage("Arco añadido", 2500);
                        }

                        arcStep = 0;
                        clearArcPreview();
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
            }
        }

        // ============================================================
        //                ARRASTRAR LÍNEA (mientras se dibuja)
        // ============================================================
        if (event->type() == QEvent::MouseMove && drawingLine)
        {
            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            QPointF p = view->mapToScene(e->pos());

            if (tempLine) {
                QPointF end;
                Tool *t = findNearestPlacedTool(p);
                if (t)
                    end = t->projectPoint(p, Tool::EdgeTop);
                else
                    end = p;
                tempLine->setLine(QLineF(lineStart, end));
                return true;
            }
        }

        // ============================================================
        //                PREVIEW DINÁMICO PARA ARCO (mientras se dibuja)
        // ============================================================
        if (event->type() == QEvent::MouseMove && currentTool == TOOL_COMPAS)
        {
            // Verificación de seguridad: solo procesar si el estado es válido
            if (arcStep < 1 || arcStep > 2) return true;

            QMouseEvent *e = static_cast<QMouseEvent*>(event);
            QPointF p = view->mapToScene(e->pos());

            // ─────────────────────────────
            // Paso 1 → solo centro fijado
            // ─────────────────────────────
            if (arcStep == 1)
            {
                double r = QLineF(arcA, p).length();
                if (r < 5) return true;

                QRectF rect(arcA.x()-r, arcA.y()-r, 2*r, 2*r);
                QPainterPath path;
                path.addEllipse(rect);

                if (!tempArc) {
                    QPen previewPen(activeColor, activeWidth);
                    previewPen.setStyle(Qt::DashLine);
                    tempArc = scene->addPath(path, previewPen);
                } else {
                    tempArc->setPath(path);
                }

                return true;
            }

            // ─────────────────────────────
            // Paso 2 → arco dinámico
            // ─────────────────────────────
            if (arcStep == 2)
            {
                double r = QLineF(arcA, arcB).length();
                if (r < 5) return true;

                double startAngle = QLineF(arcA, arcB).angle();
                double endAngle   = QLineF(arcA, p).angle();
                double span = endAngle - startAngle;

                // Normalizar el span para que esté entre -180 y 180 grados
                // Esto permite arcos en ambos sentidos: positivo = horario, negativo = antihorario
                while (span > 180) span -= 360;
                while (span < -180) span += 360;

                QRectF rect(arcA.x()-r, arcA.y()-r, 2*r, 2*r);
                QPainterPath path;
                path.arcMoveTo(rect, startAngle);
                path.arcTo(rect, startAngle, span);

                if (!tempArc) {
                    QPen previewPen(activeColor, activeWidth);
                    previewPen.setStyle(Qt::DashLine);
                    tempArc = scene->addPath(path, previewPen);
                } else {
                    tempArc->setPath(path);
                }

                return true;
            }
        }


        if (event->type() == QEvent::MouseButtonRelease && drawingLine)
        {
            drawingLine = false;
            tempLine = nullptr;
            return true;
        }

        // --- Botón derecho soltado ---
        // if (event->type() == QEvent::MouseButtonRelease)
        // {
        //     QMouseEvent *e = static_cast<QMouseEvent*>(event);
        //
        //     if (e->button() == Qt::RightButton)
        //     {
        //         QMouseEvent fakeRelease(
        //             QEvent::MouseButtonRelease,
        //             e->pos(),
        //             e->globalPosition().toPoint(),  // Qt6
        //             Qt::LeftButton,
        //             Qt::LeftButton,
        //             e->modifiers()
        //             );
        //         QApplication::sendEvent(obj, &fakeRelease);
        //
        //         view->setDragMode(QGraphicsView::NoDrag);
        //         return true;
        //     }
        // }
    }

    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::onTogglePassword(bool checked)
{
    ui->line_contra->setEchoMode(
        checked ? QLineEdit::Normal : QLineEdit::Password
        );
    ui->btnShowPassword->setIcon(
        QIcon(checked ? ":icon/resources/icons/eye-open.png" : ":icon/resources/icons/eye-closed.svg")
        );
}

void MainWindow::clearArcPreview()
{
    if (tempArc) {
        scene->removeItem(tempArc);
        delete tempArc; tempArc = nullptr;
    }
    if (arcMarkerCenter) { scene->removeItem(arcMarkerCenter); delete arcMarkerCenter; arcMarkerCenter = nullptr; }
    if (arcMarkerStart) { scene->removeItem(arcMarkerStart); delete arcMarkerStart; arcMarkerStart = nullptr; }
    if (arcMarkerEnd) { scene->removeItem(arcMarkerEnd); delete arcMarkerEnd; arcMarkerEnd = nullptr; }
}

void MainWindow::onAnswerSelected()
{
    bool anyChecked = ui->radioBtnA->isChecked() ||
                      ui->radioBtnB->isChecked() ||
                      ui->radioBtnC->isChecked() ||
                      ui->radioBtnD->isChecked();

    // Habilitar el botón cuando haya alguna opción marcada. La comprobación de validez
    // de la pregunta se hace al pulsar "Corregir".
    ui->btnCorregir->setEnabled(anyChecked);
}
