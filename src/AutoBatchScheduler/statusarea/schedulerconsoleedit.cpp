/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
