#include "sleepwidget.h"
#include "ui_sleepwidget.h"

SleepWidget::SleepWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SleepWidget)
{
    ui->setupUi(this);
}

SleepWidget::~SleepWidget()
{
    delete ui;
}
