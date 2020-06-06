#include "ffprobeextpidwidget.h"
#include "ui_ffprobeextpidwidget.h"

FFprobeExtPidWidget::FFprobeExtPidWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FFprobeExtPidWidget)
{
    ui->setupUi(this);
}

FFprobeExtPidWidget::~FFprobeExtPidWidget()
{
    delete ui;
}
