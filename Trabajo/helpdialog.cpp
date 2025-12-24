#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QSettings>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    setWindowTitle("Ayuda y Controles");

    loadKeyBindings();

    connect(this, &QDialog::accepted, this, &HelpDialog::saveKeyBindings);

    // Contenido de la guía de usuario
    ui->textBrowserGuia->setHtml(
        "<h1>Guía de Usuario</h1>"
        "<p>Bienvenido a la aplicación de cartas náuticas. Esta aplicación te permite interactuar con un mapa náutico y utilizar diversas herramientas para resolver problemas de navegación.</p>"
        "<h2>Funcionalidades Principales:</h2>"
        "<ul>"
        "<li><b>Navegación en el mapa:</b> Utiliza el ratón para arrastrar el mapa. Usa la rueda del ratón o los botones de zoom para acercar o alejar.</li>"
        "<li><b>Herramientas de dibujo:</b> Puedes añadir puntos, líneas y texto en el mapa para marcar posiciones o trazar rumbos.</li>"
        "<li><b>Herramientas de medición:</b> Usa el compás, la regla y el transportador de ángulos para realizar mediciones precisas sobre el mapa.</li>"
        "<li><b>Gestión de problemas:</b> Accede a la sección de problemas para resolver ejercicios prácticos de navegación.</li>"
        "<li><b>Perfil de usuario:</b> Gestiona tu información de perfil y consulta tu historial.</li>"
        "</ul>"
    );

    // Contenido de la sección de controles
    ui->textBrowserControles->setHtml(
        "<h1>Controles de Herramientas</h1>"
        "<p>A continuación se describen los controles básicos para las herramientas:</p>"
        "<ul>"
        "<li><b>Mover Mapa:</b> Haz clic y arrastra con el botón izquierdo del ratón.</li>"
        "<li><b>Zoom:</b> Usa la rueda del ratón o los botones +/-.</li>"
        "<li><b>Añadir Punto:</b> Selecciona la herramienta 'Punto' y haz clic en el mapa.</li>"
        "<li><b>Trazar Línea:</b> Selecciona la herramienta 'Línea', haz clic en el punto de inicio y luego en el punto final.</li>"
        "<li><b>Compás:</b> Selecciona la herramienta, haz clic para fijar el centro y arrastra para definir el radio.</li>"
        "<li><b>Regla:</b> Selecciónala para ayudarte a trazar líneas rectas sobre la carta.</li>"
        "<li><b>Transportador:</b> Selecciónalo y úsalo para medir ángulos sobre el mapa.</li>"
        "<li><b>Rotación:</b> Tanto la regla como el transportador pueden rotarse sobre cualquier punto manteniendo pulsada la tecla <b>Shift</b> y usando la <b>rueda del ratón</b>.</li>"
        "</ul>"
    );
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::saveKeyBindings()
{
    QSettings settings("MySoft", "NauticalChartApp");
    settings.beginGroup("KeyBindings");
    settings.setValue("Regla", ui->keyRegla->keySequence());
    settings.setValue("Transportador", ui->keyTransportador->keySequence());
    settings.setValue("Compas", ui->keyCompas->keySequence());
    settings.setValue("Punto", ui->keyPunto->keySequence());
    settings.setValue("Linea", ui->keyLinea->keySequence());
    settings.setValue("Texto", ui->keyTexto->keySequence());
    settings.setValue("Color", ui->keyColor->keySequence());
    settings.setValue("Borrar", ui->keyBorrar->keySequence());
    settings.setValue("Mover", ui->keyMover->keySequence());
    settings.setValue("Limpiar", ui->keyLimpiar->keySequence());
    settings.endGroup();
}

void HelpDialog::loadKeyBindings()
{
    QSettings settings("MySoft", "NauticalChartApp");
    settings.beginGroup("KeyBindings");
    ui->keyRegla->setKeySequence(settings.value("Regla", "R").value<QKeySequence>());
    ui->keyTransportador->setKeySequence(settings.value("Transportador", "Shift+T").value<QKeySequence>());
    ui->keyCompas->setKeySequence(settings.value("Compas", "C").value<QKeySequence>());
    ui->keyPunto->setKeySequence(settings.value("Punto", "P").value<QKeySequence>());
    ui->keyLinea->setKeySequence(settings.value("Linea", "L").value<QKeySequence>());
    ui->keyTexto->setKeySequence(settings.value("Texto", "T").value<QKeySequence>());
    ui->keyColor->setKeySequence(settings.value("Color", "Shift+C").value<QKeySequence>());
    ui->keyBorrar->setKeySequence(settings.value("Borrar", "B").value<QKeySequence>());
    ui->keyMover->setKeySequence(settings.value("Mover", "M").value<QKeySequence>());
    ui->keyLimpiar->setKeySequence(settings.value("Limpiar", "Delete").value<QKeySequence>());
    settings.endGroup();
}
