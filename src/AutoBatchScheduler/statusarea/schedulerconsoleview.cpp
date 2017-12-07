#include "schedulerconsoleview.h"

SchedulerConsoleView::SchedulerConsoleView(QWidget *parent)
    : ConsoleBase(parent)
{
    objname = "";
}

SchedulerConsoleView::~SchedulerConsoleView()
{

}

void SchedulerConsoleView::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    connect(task, &TaskSchedulerConnector::processStarted, this, &SchedulerConsoleView::startedMessage);
    connect(task, &TaskSchedulerConnector::processPaused, this, &SchedulerConsoleView::pausedMessage);
    connect(task, &TaskSchedulerConnector::processStopped, this, &SchedulerConsoleView::stoppedMessage);
    connect(task, &TaskSchedulerConnector::processEnd, this, &SchedulerConsoleView::endMessage);
    connect(task, &TaskSchedulerConnector::processMessage, this, &SchedulerConsoleView::updateMessage);
    connect(task, &TaskSchedulerConnector::processErrorText, this, &SchedulerConsoleView::errorMessage);

}

void SchedulerConsoleView::setReadObjectName(QString objname)
{
    this->objname = objname;
}

void SchedulerConsoleView::startedMessage(QString obj, int runtype)
{
    if(objname == obj) this->started(runtype);
}

void SchedulerConsoleView::pausedMessage(QString obj)
{
    if(objname == obj) this->pause();
}

void SchedulerConsoleView::stoppedMessage(QString obj)
{
    if(objname == obj) this->stop();
}

void SchedulerConsoleView::endMessage(QString obj, int runtype)
{
    if(objname == obj) this->end(runtype);
}

void SchedulerConsoleView::updateMessage(QString obj, QString data, int runtype)
{
    if(objname == obj) this->updateText(data, runtype);
}

void SchedulerConsoleView::errorMessage(QString obj, QString str)
{
    if(objname == obj) this->updateExternalError(str);
}
