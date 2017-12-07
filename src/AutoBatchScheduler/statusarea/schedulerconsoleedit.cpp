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
    connect(this, &SchedulerConsoleEdit::returnPressed, this, &SchedulerConsoleEdit::sendMessage);
}

void SchedulerConsoleEdit::sendMessage()
{
    QString text = this->text();
    if(text == ""){
#ifdef Q_OS_WIN
        taskc->sendInput(this->objectName(), "\n");
#else
        taskc->sendInput(this->objectName(), "\r\n");
#endif
    }else{
        taskc->sendInput(this->objectName(), text);
        this->clear();
    }
}
