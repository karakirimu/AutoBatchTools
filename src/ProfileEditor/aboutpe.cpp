#include "aboutpe.h"
#include "ui_aboutpe.h"

AboutPE::AboutPE(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPE)
{
    ui->setupUi(this);
}

AboutPE::~AboutPE()
{
    delete ui;
}
