/*
 * Copyright 2016-2021 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
