#ifndef AUTOSAVESELECTIONDIALOG_H
#define AUTOSAVESELECTIONDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QDateTime>

namespace Ui {
class AutoSaveSelectionDialog;
}

class AutoSaveSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoSaveSelectionDialog(QWidget *parent = nullptr);
    ~AutoSaveSelectionDialog();

    void setAutoSaveFileList(QStringList *list);
    QString execDialog();

private:
    Ui::AutoSaveSelectionDialog *ui;
    QStringList *filelist;
};

#endif // AUTOSAVESELECTIONDIALOG_H
