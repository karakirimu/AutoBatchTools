#include "consolewidget.h"
#include "ui_consolewidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);

    window()->setAttribute(Qt::WA_DeleteOnClose);
}

ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}

void ConsoleWidget::setConsoleViewTarget(TaskSchedulerConnector *taskc, QString objname)
{
    ui->console->setTaskSchedulerConnector(taskc);
    ui->console->setReadObjectName(objname);
}
