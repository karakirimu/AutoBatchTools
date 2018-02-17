#include "commandlinetextedit.h"

CommandLineTextEdit::CommandLineTextEdit(QObject *)
{
}

CommandLineTextEdit::~CommandLineTextEdit()
{

}

void CommandLineTextEdit::getUpdateStream(QString type, QString data)
{
    if(type == "running"){
        this->setText(data);
    }
}


