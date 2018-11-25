#include "consoleview.h"

ConsoleView::ConsoleView(QWidget *parent)
    : ConsoleBase(parent)
{
}

ConsoleView::~ConsoleView()
{
}

void ConsoleView::setMultiTask(MultiTaskP *mlTask)
{
    connect(mlTask, &MultiTaskP::processStarted, this, &ConsoleView::startedMessage);
    connect(mlTask, &MultiTaskP::processPaused, this, &ConsoleView::pausedMessage);
    connect(mlTask, &MultiTaskP::processStopped, this, &ConsoleView::stoppedMessage);
    connect(mlTask, &MultiTaskP::processEnd, this, &ConsoleView::endMessage);
    connect(mlTask, &MultiTaskP::processMessage, this, &ConsoleView::updateMessage);
    connect(mlTask, &MultiTaskP::processErrorText, this, &ConsoleView::errorMessage);

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
