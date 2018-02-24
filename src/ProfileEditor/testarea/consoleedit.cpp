#include "consoleedit.h"

ConsoleEdit::ConsoleEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

ConsoleEdit::~ConsoleEdit()
{

}

void ConsoleEdit::setMultiTask(MultiTaskP *rbinder)
{
    binder = rbinder;
    connect(this, &ConsoleEdit::returnPressed, this, &ConsoleEdit::sendMessage);
}

void ConsoleEdit::sendMessage()
{
    QString text = this->text();
    if(text == ""){
#ifdef Q_OS_WIN
        binder->sendInput(this->objectName(), "\n");
#else
        binder->sendInput(this->objectName(), "\r\n");
#endif
    }else{
        binder->sendInput(this->objectName(), text);
        this->clear();
    }
}
