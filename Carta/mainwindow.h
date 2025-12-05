#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsLineItem>
#include "tool.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;



private slots:
    void zoomIn();
    void zoomOut();

    void setDrawLineMode(bool enabled);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsView *view;

    double m_scaleFactor = 1.0;

    Tool* m_protractor;       // ahora es un QGraphicsSvgItem dentro de la escena

    QAction *m_actDrawLine = nullptr;
    bool m_drawLineMode = false;
    QGraphicsLineItem *m_tempLine = nullptr;
    QGraphicsLineItem *m_currentLineItem = nullptr;
    QPointF m_lineStart;

    void applyZoom(double factor);
};

#endif // MAINWINDOW_H
