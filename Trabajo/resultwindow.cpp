#include "resultwindow.h"
#include "navigation.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateEdit>
#include <QPushButton>

ResultsWindow::ResultsWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Resultados");

    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    QStringList headers = {"Fecha", "Aciertos", "Fallos"};
    m_table->setHorizontalHeaderLabels(headers);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_deFrom = new QDateEdit(this);
    m_deFrom->setCalendarPopup(true);
    m_deFrom->setDate(QDate::currentDate().addDays(-30));

    m_btnFilter = new QPushButton("Filtrar", this);
    connect(m_btnFilter, &QPushButton::clicked, this, &ResultsWindow::onFilter);

    auto *lay = new QVBoxLayout(this);
    lay->addWidget(m_deFrom);
    lay->addWidget(m_btnFilter);
    lay->addWidget(m_table);

    loadSessions();
    resize(600, 400);
}

void ResultsWindow::onFilter()
{
    loadSessions();
}

void ResultsWindow::loadSessions()
{
    m_table->setRowCount(0);
    // Obtener sesiones del usuario actual y mostrarlas.
    // Aquí necesitas el usuario autenticado; por simplicidad mostramos una tabla vacía.
    // Integrar con Navigation::findUser(nick)->sessions() cuando dispongas del nick.
}
