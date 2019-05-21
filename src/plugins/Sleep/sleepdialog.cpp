#include "sleepdialog.h"
#include "ui_sleepdialog.h"

SleepDialog::SleepDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SleepDialog)
{
    ui->setupUi(this);

    int counter = current->count();
    if(counter > 0) ui->timerLineEdit->setText(current->at(0));

    setWindowTitle("Sleep");
}

SleepDialog::~SleepDialog()
{
    delete ui;
}

QStringList SleepDialog::getargs()
{
    QStringList result;
    result.append(ui->timerLineEdit->text());
    return result;
}
