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
    connect(this, &ConsoleEdit::returnPressed, this, &ConsoleEdit::sendMessage);
}

void ConsoleEdit::sendMessage()
{
    QString text = this->text();
    if(text == ""){
        mlTask->sendInput(this->objectName(), "\n");

    }else{
        mlTask->sendInput(this->objectName(), text);
        this->clear();

    }
}
