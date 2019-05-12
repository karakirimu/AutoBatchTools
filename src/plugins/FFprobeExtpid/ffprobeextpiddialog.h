#ifndef FFPROBEEXTPIDDIALOG2_H
#define FFPROBEEXTPIDDIALOG2_H

#include <QDialog>

namespace Ui {
class FFprobeExtpidDialog2;
}

class FFprobeExtpidDialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit FFprobeExtpidDialog2(QWidget *parent = nullptr);
    ~FFprobeExtpidDialog2();

private:
    Ui::FFprobeExtpidDialog2 *ui;
};

#endif // FFPROBEEXTPIDDIALOG2_H
