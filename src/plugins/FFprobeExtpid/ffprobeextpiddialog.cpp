#include "ffprobeextpiddialog.h"
#include "ui_ffprobeextpiddialog.h"

FFprobeExtpidDialog::FFprobeExtpidDialog(QStringList *current, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FFprobeExtpidDialog)
{
    ui->setupUi(this);

    //set dialog data
    int counter = current->count();
    if(counter > 0) ui->input->setText(current->at(0));
    if(counter > 1) ui->out_videopid->setText(current->at(1));
    if(counter > 2) ui->out_audiopid->setText(current->at(2));

}

FFprobeExtpidDialog::~FFprobeExtpidDialog()
{
    delete ui;
}

QStringList FFprobeExtpidDialog::getargs()
{
    QStringList result;
    result.append(ui->input->text());
    result.append(ui->out_videopid->text());
    result.append(ui->out_audiopid->text());

    return result;
}
