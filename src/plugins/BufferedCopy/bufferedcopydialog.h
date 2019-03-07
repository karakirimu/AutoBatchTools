#ifndef BUFFEREDCOPYDIALOG_H
#define BUFFEREDCOPYDIALOG_H

#include <QObject>
#include <QtWidgets/QDialog>

namespace Ui {
class BufferedCopyDialog;
}

class BufferedCopyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BufferedCopyDialog(QStringList *current, QWidget *parent = nullptr);
    ~BufferedCopyDialog();

    QStringList getargs();

private:
    Ui::BufferedCopyDialog *ui;
};

#endif // BUFFEREDCOPYDIALOG_H
