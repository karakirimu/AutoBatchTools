#include "takelinedialog.h"
#include "ui_takelinedialog.h"

TakeLineDialog::TakeLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TakeLineDialog)
{
    ui->setupUi(this);
}

TakeLineDialog::~TakeLineDialog()
{
    delete ui;
}
