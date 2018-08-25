#include "takelinedialog.h"
#include "ui_takelinedialog.h"

TakeLineDialog::TakeLineDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TakeLineDialog)
{
    ui->setupUi(this);

    int counter = current->count();
    if(counter > 0) ui->plainTextEdit->setPlainText(current->at(0));
    if(counter > 1) ui->removelineEdit->setText(current->at(1));
    if(counter > 2) ui->returnLineEdit->setText(current->at(2));

    setWindowTitle("TakeLine");
}

TakeLineDialog::~TakeLineDialog()
{
    delete ui;
}

QStringList TakeLineDialog::getargs()
{
    QStringList result;
    result.append(ui->plainTextEdit->toPlainText());
    result.append(ui->removelineEdit->text());
    result.append(ui->returnLineEdit->text());
    return result;
}
