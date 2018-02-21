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
#ifdef Q_OS_WIN
        mlTask->sendInput(this->objectName(), "\n");
#else
        binder->sendInput(this->objectName(), "\r\n");
#endif
    }else{
        mlTask->sendInput(this->objectName(), text);
        this->clear();
    }
}
