/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "consoleview.h"

ConsoleView::ConsoleView(QObject *)
{
}

ConsoleView::~ConsoleView()
{

}

void ConsoleView::setMultiTask(MultiTask *mltask)
{
    connect(mltask, &MultiTask::processStarted, this, &ConsoleView::startedMessage);
    connect(mltask, &MultiTask::processPaused, this, &ConsoleView::pausedMessage);
    connect(mltask, &MultiTask::processStopped, this, &ConsoleView::stoppedMessage);
    connect(mltask, &MultiTask::processEnd, this, &ConsoleView::endMessage);
    connect(mltask, &MultiTask::processMessage, this, &ConsoleView::updateMessage);
    connect(mltask, &MultiTask::processErrorText, this, &ConsoleView::errorMessage);
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
