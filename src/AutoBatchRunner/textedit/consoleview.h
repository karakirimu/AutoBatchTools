#ifndef COMMANDLINETEXTEDIT_H
#define COMMANDLINETEXTEDIT_H

#include <QEvent>
#include <QTextEdit>
#include <QWidget>
#include <QDebug>

class CommandLineTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CommandLineTextEdit(QObject *parent = 0);
    ~CommandLineTextEdit();

public slots:
    void getUpdateStream(QString type, QString data);

};

#endif // COMMANDLINETEXTEDIT_H
