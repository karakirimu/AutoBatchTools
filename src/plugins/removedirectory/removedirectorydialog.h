#ifndef REMOVEDIRECTORYDIALOG_H
#define REMOVEDIRECTORYDIALOG_H

#include <QDialog>

namespace Ui {
class RemoveDirectoryDialog;
}

class RemoveDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDirectoryDialog(QStringList *current, QWidget *parent = nullptr);
    ~RemoveDirectoryDialog();

    QStringList getargs();

private:
    Ui::RemoveDirectoryDialog *ui;
};

#endif // REMOVEDIRECTORYDIALOG_H
