#include "consolewidget.h"
#include "ui_consolewidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);
}

ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}
