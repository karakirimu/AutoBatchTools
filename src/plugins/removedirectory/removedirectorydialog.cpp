#include "removedirectorydialog.h"
#include "ui_removedirectorydialog.h"

RemoveDirectoryDialog::RemoveDirectoryDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveDirectoryDialog)
{
    ui->setupUi(this);
    //set arguments to this dialog
    int counter = current->count();
    if(counter > 0) ui->rmpathLineEdit->setText(current->at(0));
    if(counter > 1) {
        QString argcheck = static_cast<QString>(current->at(1)).toLower();
        if (argcheck == "-rf"){
            ui->fileRemoveCheckBox->setChecked(true);
        }
    }
    setWindowTitle("RemoveDirectory");
}

RemoveDirectoryDialog::~RemoveDirectoryDialog()
{
    delete ui;
}

QStringList RemoveDirectoryDialog::getargs()
{
    QStringList result;
    result.append(ui->rmpathLineEdit->text());

    if(ui->fileRemoveCheckBox->isChecked()){
        result.append("-rf");
    }

    return result;
}
