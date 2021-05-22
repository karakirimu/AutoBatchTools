/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "consoleedit.h"

ConsoleEdit::ConsoleEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

ConsoleEdit::~ConsoleEdit()
{

}

void ConsoleEdit::setMultiTask(MultiTask *mltask)
{
    mlTask = mltask;
}

void ConsoleEdit::keyPressEvent(QKeyEvent *keyevent)
{
    if((keyevent->key() == Qt::Key_Enter
         || keyevent->key() == Qt::Key_Return)
        && keyevent->modifiers() == Qt::ControlModifier){
        sendMessage();
        return;
    }

    QLineEdit::keyPressEvent(keyevent);
}

void ConsoleEdit::sendMessage()
{
    QString text = this->text();
    if(text == ""){
        mlTask->sendInput(this->objectName(), "\n");

    }else{
        mlTask->sendInput(this->objectName(), text + "\n");
        this->clear();
    }
}
