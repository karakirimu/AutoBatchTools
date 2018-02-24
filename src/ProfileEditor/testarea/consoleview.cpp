#include "consoleview.h"

ConsoleView::ConsoleView(QWidget *parent)
    : ConsoleBase(parent)
{
}

ConsoleView::~ConsoleView()
{
}

void ConsoleView::setMultiTask(MultiTask *mlTask)
{
    connect(mlTask, &MultiTask::processStarted, this, &ConsoleView::startedMessage);
    connect(mlTask, &MultiTask::processPaused, this, &ConsoleView::pausedMessage);
    connect(mlTask, &MultiTask::processStopped, this, &ConsoleView::stoppedMessage);
    connect(mlTask, &MultiTask::processEnd, this, &ConsoleView::endMessage);
    connect(mlTask, &MultiTask::processMessage, this, &ConsoleView::updateMessage);
    connect(mlTask, &MultiTask::processErrorText, this, &ConsoleView::errorMessage);

}

void ConsoleView::setReadObjectName(QString objname)
{
    this->objname = objname;
}

void ConsoleView::startedMessage(QString obj, int runtype)
{
    if(objname == obj) this->started(runtype);
}

void ConsoleView::pausedMessage(QString obj)
{
    if(objname == obj) this->pause();
}

void ConsoleView::stoppedMessage(QString obj)
{
    if(objname == obj) this->stop();
}

void ConsoleView::endMessage(QString obj, int runtype)
{
    if(objname == obj) this->end(runtype);
}

void ConsoleView::updateMessage(QString obj, QString data, int runtype)
{
    if(objname == obj) this->updateText(data, runtype);
}

void ConsoleView::errorMessage(QString obj, QString str)
{
    if(objname == obj) this->updateExternalError(str);
}
