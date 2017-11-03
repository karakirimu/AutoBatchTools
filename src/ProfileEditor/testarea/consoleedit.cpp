#include "consoleedit.h"

ConsoleEdit::ConsoleEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

ConsoleEdit::~ConsoleEdit()
{

}

void ConsoleEdit::setRunTaskSignalBinder(RunTaskSignalBinder *rbinder)
{
    binder = rbinder;
    connect(this, &ConsoleEdit::returnPressed, this, &ConsoleEdit::sendMessage);
}

void ConsoleEdit::sendMessage()
{
    QString text = this->text();
    if(text == ""){
#ifdef Q_OS_WIN
        binder->sendInput("\n");
#else
        binder->sendInput("\r\n");
#endif
    }else{
        binder->sendInput(text);
        this->clear();
    }
}
