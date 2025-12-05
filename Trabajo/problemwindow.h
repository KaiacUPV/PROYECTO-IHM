#pragma once
#include <QWidget>

class QLabel;
class QPushButton;

class ProblemWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemWindow(QWidget *parent = nullptr);

private slots:
    void onCheckAnswer(int idx);
    void loadRandomProblem();

private:
    QLabel *m_lblQuestion;
    QList<QPushButton*> m_answerButtons;
    int m_currentCorrectIndex = -1;
};
