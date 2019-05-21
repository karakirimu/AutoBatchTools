#ifndef SLEEPDIALOG_H
#define SLEEPDIALOG_H

#include <QDialog>

namespace Ui {
class SleepDialog;
}

class SleepDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SleepDialog(QStringList *current, QWidget *parent = nullptr);
    ~SleepDialog();

    QStringList getargs();

private:
    Ui::SleepDialog *ui;
};

#endif // SLEEPDIALOG_H
