#ifndef REGEXPINPUTRENAMEDIALOG_H
#define REGEXPINPUTRENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class RegExpInputRenameDialog;
}

class RegExpInputRenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegExpInputRenameDialog(QWidget *parent = nullptr);
    ~RegExpInputRenameDialog();

private:
    Ui::RegExpInputRenameDialog *ui;
};

#endif // REGEXPINPUTRENAMEDIALOG_H
