#include "removedirectorydialog.h"
#include "ui_removedirectorydialog.h"

RemoveDirectoryDialog::RemoveDirectoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveDirectoryDialog)
{
    ui->setupUi(this);
}

RemoveDirectoryDialog::~RemoveDirectoryDialog()
{
    delete ui;
}
