#include "filetextreplacedialog.h"
#include "ui_filetextreplacedialog.h"

FileTextReplaceDialog::FileTextReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileTextReplaceDialog)
{
    ui->setupUi(this);
}

FileTextReplaceDialog::~FileTextReplaceDialog()
{
    delete ui;
}
