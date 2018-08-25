#ifndef TAKELINEDIALOG_H
#define TAKELINEDIALOG_H

#include <QObject>
#include <QtWidgets/QDialog>

namespace Ui {
class TakeLineDialog;
}

class TakeLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TakeLineDialog(QStringList *current, QWidget *parent = 0);
    ~TakeLineDialog();

    QStringList getargs();

private:
    Ui::TakeLineDialog *ui;

};

#endif // TAKELINEDIALOG_H
