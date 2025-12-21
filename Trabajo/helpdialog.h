#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QKeySequence>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private slots:
    void saveKeyBindings();
    void loadKeyBindings();

private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
