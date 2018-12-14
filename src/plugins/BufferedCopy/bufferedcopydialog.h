#ifndef BUFFEREDCOPYDIALOG_H
#define BUFFEREDCOPYDIALOG_H

#include <QDialog>

namespace Ui {
class BufferedCopyDialog;
}

class BufferedCopyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BufferedCopyDialog(QWidget *parent = nullptr);
    ~BufferedCopyDialog();

private:
    Ui::BufferedCopyDialog *ui;
};

#endif // BUFFEREDCOPYDIALOG_H
