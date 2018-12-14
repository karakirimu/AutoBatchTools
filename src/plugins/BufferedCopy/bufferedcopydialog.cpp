#include "bufferedcopydialog.h"
#include "ui_bufferedcopydialog.h"

BufferedCopyDialog::BufferedCopyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BufferedCopyDialog)
{
    ui->setupUi(this);
}

BufferedCopyDialog::~BufferedCopyDialog()
{
    delete ui;
}
