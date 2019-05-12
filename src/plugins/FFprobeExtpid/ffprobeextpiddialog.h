#ifndef FFPROBEEXTPIDDIALOG2_H
#define FFPROBEEXTPIDDIALOG2_H

#include <QDialog>

namespace Ui {
class FFprobeExtpidDialog;
}

class FFprobeExtpidDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FFprobeExtpidDialog(QStringList *current, QWidget *parent = nullptr);
    ~FFprobeExtpidDialog();

    QStringList getargs();

private:
    Ui::FFprobeExtpidDialog *ui;
};

#endif // FFPROBEEXTPIDDIALOG2_H
