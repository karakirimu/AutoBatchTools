#include "ffprobeextpiddialog2.h"
#include "ui_ffprobeextpiddialog2.h"

FFprobeExtpidDialog2::FFprobeExtpidDialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FFprobeExtpidDialog2)
{
    ui->setupUi(this);
}

FFprobeExtpidDialog2::~FFprobeExtpidDialog2()
{
    delete ui;
}
