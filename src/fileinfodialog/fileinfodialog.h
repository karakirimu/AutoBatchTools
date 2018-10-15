#ifndef FILEINFODIALOG_H
#define FILEINFODIALOG_H

#include "fileinfodialog_global.h"
#include <QDialog>
#include <QFileInfo>
#include <QDateTime>

namespace Ui {
class FileInfoDialog;
}

class FILEINFODIALOGSHARED_EXPORT FileInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileInfoDialog(QWidget *parent = nullptr);
    ~FileInfoDialog();

    void setFileInfo(QString file);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::FileInfoDialog *ui;
    QIcon icon();
};

#endif // FILEINFODIALOG_H
