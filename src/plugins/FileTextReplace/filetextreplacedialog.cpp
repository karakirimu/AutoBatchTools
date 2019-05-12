#include "filetextreplacedialog.h"
#include "ui_filetextreplacedialog.h"

FileTextReplaceDialog::FileTextReplaceDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileTextReplaceDialog)
{
    ui->setupUi(this);

    //set dialog data
    int counter = current->count();
    if(counter > 0) ui->inputEdit->setText(current->at(0));
    if(counter > 1) ui->outputEdit->setText(current->at(1));
    if(counter > 2) ui->searchEdit->setText(current->at(2));
    if(counter > 3) ui->replaceEdit->setText(current->at(3));

}

FileTextReplaceDialog::~FileTextReplaceDialog()
{
    delete ui;
}

QStringList FileTextReplaceDialog::getargs()
{
    QStringList result;
    result.append(ui->inputEdit->text());
    result.append(ui->outputEdit->text());
    result.append(ui->searchEdit->text());
    result.append(ui->replaceEdit->text());

    return result;
}
