#include "removedirectrywidget.h"
#include "ui_removedirectrywidget.h"

RemoveDirectryWidget::RemoveDirectryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoveDirectryWidget)
{
    ui->setupUi(this);
}

RemoveDirectryWidget::~RemoveDirectryWidget()
{
    delete ui;
}
