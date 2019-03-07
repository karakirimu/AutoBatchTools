#include "bufferedcopydialog.h"
#include "ui_bufferedcopydialog.h"

BufferedCopyDialog::BufferedCopyDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BufferedCopyDialog)
{
    ui->setupUi(this);

    int counter = current->count();
    if(counter > 0) ui->lineEditSource->setText(current->at(0));
    if(counter > 1) ui->lineEditDestination->setText(current->at(1));

    QString argcheck;
    for(int i = 2; i < counter; i++){
        argcheck = static_cast<QString>(current->at(i)).toLower();

        if(argcheck == "-rename"){
            ui->checkBoxRename->setChecked(true);

            if(/*argcheck != "-cs"
                    && argcheck != "-v"
                    && argcheck != "-rename"
                    &&*/ (i + 1) < counter){
                ui->lineEditRename->setText(current->at(i + 1));
                i++;
            }
        }

        if(argcheck == "-cs"){
            ui->checkBoxCache->setChecked(true);

            if(/*argcheck != "-cs"
                    && argcheck != "-v"
                    && argcheck != "-rename"
                    &&*/ (i + 1) < counter){
                ui->lineEditCache->setText(current->at(i + 1));
                i++;
            }
        }

        if(argcheck == "-v"){
            ui->checkBoxVerify->setChecked(true);
        }
    }

    setWindowTitle("BufferedCopy");
}

BufferedCopyDialog::~BufferedCopyDialog()
{
    delete ui;
}

QStringList BufferedCopyDialog::getargs()
{
    QStringList result;
    result.append(ui->lineEditSource->text());
    result.append(ui->lineEditDestination->text());
    if(ui->checkBoxRename->isChecked()){
        result.append("-rename");
        result.append(ui->lineEditRename->text());
    }

    if(ui->checkBoxCache->isChecked()){
        result.append("-cs");
        result.append(ui->lineEditCache->text());
    }

    if(ui->checkBoxVerify->isChecked()){
        result.append("-v");
    }

    return result;
}
