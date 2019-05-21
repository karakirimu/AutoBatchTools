#include "sleepdialog.h"
#include "ui_sleepdialog.h"

SleepDialog::SleepDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SleepDialog)
{
    ui->setupUi(this);
}

SleepDialog::~SleepDialog()
{
    delete ui;
}
