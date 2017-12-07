#ifndef SCHEDULERCONSOLEVIEW_H
#define SCHEDULERCONSOLEVIEW_H

#include <QWidget>
#include <taskschedulerconnector.h>
#include <../ConsoleBase/consolebase.h>

class SchedulerConsoleView : public ConsoleBase
{
public:
    explicit SchedulerConsoleView(QWidget *parent = nullptr);
    ~SchedulerConsoleView();

    //ui settings
    void setTaskSchedulerConnector(TaskSchedulerConnector *task);
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

#endif // SCHEDULERCONSOLEVIEW_H
