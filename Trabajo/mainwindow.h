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
#include <QSplitter>
#include <QPixmap>
#include <QColor>
#include <QPointF>

#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include "tool.h"
#include "navigation.h"

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
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    // Panel usuario / problemas
    void onPerfil();
    void onProblemas();
    void onEditAvatar();
    void onSaveProfile();
    void onCancelProfile();
    void onLogout();
    void onlogin();
    void back();

    // Problemas
    void initializeProblems();
    void loadProblem(int index);
    void on_btnAleatorio_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_btnCorregir_clicked();
    void onAnswerSelected();

    // Herramientas
    void onTexto();
    void onPunto();
    void onLinea();
    void onArco();
    void onColor();
    void onMover();
    void onZoomIn();
    void onZoomOut();
    void onBorrar();
    void onLimpiar();
    void onRegla();
    void onCompas();
    void onTransportador();

private:
    void loadCarta();
    void updateUserAvatar();
    void centerToolOnView(Tool *tool);
    bool validateEmail(const QString &email);
    bool validatePassword(const QString &password);
    void loadProfileUI();

    void applyZoom(double factor); // added declaration

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPixmap cartaPixmap;

    // Session / user
    User m_loggedUser;
    bool m_isLogged = false;
    QString currentNickName;
    QString currentAvatarPath;
    int m_sessionHits = 0;
    int m_sessionFaults = 0;
    Session m_currentSession;

    // Graphics / tools
    double zoomLevel = 1.0;
    Tool *regla = nullptr;
    Tool *compas = nullptr;
    Tool *transportador = nullptr;

    // Drawing
    QGraphicsLineItem *tempLine = nullptr;
    bool drawingLine = false;
    QPointF lineStart;
    QColor activeColor = Qt::black;
    int activeWidth = 2;
    int activeFontSize = 12;

    // Tools enum (ensure defined here)
    enum ToolKind {
        TOOL_NONE = 0,
        TOOL_TEXTO,
        TOOL_PUNTO,
        TOOL_LINEA,
        TOOL_ARCO,
        TOOL_COLOR,
        TOOL_MOVER,
        TOOL_BORRAR
    };
    int currentTool = TOOL_NONE;
    int arcStep = 0;
    QPointF arcA, arcB;

    // Problems
    Problem m_currentProblem;
    QVector<Answer> m_currentAnswersRandom;
    int m_correctAnswerIndex = -1;
};

#endif // MAINWINDOW_H
