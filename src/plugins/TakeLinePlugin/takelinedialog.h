#ifndef TAKELINEDIALOG_H
#define TAKELINEDIALOG_H

#include <QDialog>

namespace Ui {
class TakeLineDialog;
}

class TakeLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TakeLineDialog(QWidget *parent = 0);
    ~TakeLineDialog();

private:
    Ui::TakeLineDialog *ui;
};

#endif // TAKELINEDIALOG_H
