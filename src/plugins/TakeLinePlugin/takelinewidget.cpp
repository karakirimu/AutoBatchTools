#include "takelinewidget.h"
#include "ui_takelinewidget.h"

TakeLineWidget::TakeLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TakeLineWidget)
{
    ui->setupUi(this);
}

TakeLineWidget::~TakeLineWidget()
{
    delete ui;
}
