#include "problemwindow.h"
#include "navigation.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QRandomGenerator>
#include <QMessageBox>

ProblemWindow::ProblemWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Resolver problema");
    auto *lay = new QVBoxLayout(this);
    m_lblQuestion = new QLabel(this);
    m_lblQuestion->setWordWrap(true);
    lay->addWidget(m_lblQuestion);

    for (int i=0;i<4;i++) {
        QPushButton *b = new QPushButton(this);
        connect(b, &QPushButton::clicked, this, [this, i]() { onCheckAnswer(i); });
        m_answerButtons.append(b);
        lay->addWidget(b);
    }

    loadRandomProblem();
    resize(600, 300);
}

void ProblemWindow::loadRandomProblem()
{
    const QVector<Problem> &probs = Navigation::instance().problems();
    if (probs.isEmpty()) {
        m_lblQuestion->setText("No hay problemas en la base de datos.");
        for (auto *b : m_answerButtons) b->setEnabled(false);
        return;
    }

    int idx = QRandomGenerator::global()->bounded(probs.size());
    const Problem &p = probs[idx];

    m_lblQuestion->setText(p.text());
    // copiar respuestas y mezclar
    QVector<int> order;
    for (int i=0;i<p.answers().size();++i) order.append(i);
    std::shuffle(order.begin(), order.end(), *QRandomGenerator::global());

    for (int i=0;i<4;i++) {
        int ansIdx = order[i];
        m_answerButtons[i]->setText(p.answers()[ansIdx].text());
        m_answerButtons[i]->setEnabled(true);
        if (p.answers()[ansIdx].validity()) m_currentCorrectIndex = i;
    }
}

void ProblemWindow::onCheckAnswer(int idx)
{
    bool correct = (idx == m_currentCorrectIndex);
    QMessageBox::information(this, "Resultado", correct ? "Respuesta correcta" : "Respuesta incorrecta");

    // Aquí deberías incrementar hits/faults en la sesión actual del usuario
    // Por simplicidad: dejamos esa lógica para integrar con Navigation / Session real.

    loadRandomProblem();
}
