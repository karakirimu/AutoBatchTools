#include "regexpinputrenamedialog.h"
#include "ui_regexpinputrenamedialog.h"

RegExpInputRenameDialog::RegExpInputRenameDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegExpInputRenameDialog)
{
    ui->setupUi(this);

    //set arguments to this dialog
    int counter = current->count();
    if(counter > 0) ui->InputlineEdit->setText(current->at(0));
    if(counter > 1) ui->regExplineEdit->setText(current->at(1));
    if(counter > 2) ui->replaceLineEdit->setText(current->at(2));
    if(counter > 3) ui->separatorLineEdit->setText(current->at(3));

    setWindowTitle("RegExpInputRename");

}

RegExpInputRenameDialog::~RegExpInputRenameDialog()
{
    delete ui;
}

QStringList RegExpInputRenameDialog::getargs()
{
    QStringList result;
    result.append(ui->InputlineEdit->text());
    result.append(ui->regExplineEdit->text());
    result.append(ui->replaceLineEdit->text());
    result.append(ui->separatorLineEdit->text());
    return result;
}
