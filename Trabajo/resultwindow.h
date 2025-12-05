#pragma once
#include <QWidget>

class QTableWidget;
class QDateEdit;
class QPushButton;

class ResultsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsWindow(QWidget *parent = nullptr);

private slots:
    void onFilter();
    void loadSessions();
private:
    QTableWidget *m_table;
    QDateEdit *m_deFrom;
    QPushButton *m_btnFilter;
};
