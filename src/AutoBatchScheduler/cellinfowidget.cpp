#include "cellinfowidget.h"
#include "ui_cellinfowidget.h"

cellInfoWidget::cellInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cellInfoWidget)
{
    ui->setupUi(this);
}

cellInfoWidget::~cellInfoWidget()
{
    delete ui;
}
