#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QWidget>
#include <multitaskp.h>
#include <../ConsoleBase/consolebase.h>

class ConsoleView : public ConsoleBase
{
    Q_OBJECT
public:
    explicit ConsoleView(QWidget *parent = nullptr);
    ~ConsoleView();

    //ui settings
    void setMultiTask(MultiTaskP *task);    
    void setReadObjectName(QString objname);

private slots:
    void startedMessage(QString obj, int runtype);
    void pausedMessage(QString obj);
    void stoppedMessage(QString obj);
    void endMessage(QString obj, int runtype);
    void updateMessage(QString obj, QString data, int runtype);
    void errorMessage(QString obj, QString str);

private:
    QString objname;

};

#endif // CONSOLEVIEW_H
