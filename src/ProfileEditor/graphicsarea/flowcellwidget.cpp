#include "flowcellwidget.h"
#include "ui_flowcellwidget.h"

FlowCellWidget::FlowCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlowCellWidget)
{
    ui->setupUi(this);
}

FlowCellWidget::~FlowCellWidget()
{
    delete ui;
}
