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

#include "schedulerconsoleedit.h"

SchedulerConsoleEdit::SchedulerConsoleEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

SchedulerConsoleEdit::~SchedulerConsoleEdit()
{

}

void SchedulerConsoleEdit::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
}

void SchedulerConsoleEdit::keyPressEvent(QKeyEvent *keyevent)
{
    if((keyevent->key() == Qt::Key_Enter
         || keyevent->key() == Qt::Key_Return)
        && keyevent->modifiers() == Qt::ControlModifier){
        sendMessage();
        return;
    }

    QLineEdit::keyPressEvent(keyevent);
}

void SchedulerConsoleEdit::sendMessage()
{
    QString text = this->text();
    if(text == ""){
        taskc->sendInput(this->objectName(), "\n");
    }else{
        taskc->sendInput(this->objectName(), text + "\n");
        this->clear();
    }
}
