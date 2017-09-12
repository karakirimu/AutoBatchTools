#include "mainscheduler.h"
#include "ui_mainscheduler.h"

MainScheduler::MainScheduler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScheduler)
{
    ui->setupUi(this);
}

MainScheduler::~MainScheduler()
{
    delete ui;
}
