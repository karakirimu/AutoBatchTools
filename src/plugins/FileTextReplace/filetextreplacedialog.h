#ifndef FILETEXTREPLACEDIALOG_H
#define FILETEXTREPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class FileTextReplaceDialog;
}

class FileTextReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileTextReplaceDialog(QWidget *parent = nullptr);
    ~FileTextReplaceDialog();

private:
    Ui::FileTextReplaceDialog *ui;
};

#endif // FILETEXTREPLACEDIALOG_H
