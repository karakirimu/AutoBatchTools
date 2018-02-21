#ifndef COMMANDLINETEXTEDIT_H
#define COMMANDLINETEXTEDIT_H

#include <QEvent>
#include <QWidget>
#include <multitask.h>
#include <../ConsoleBase/consolebase.h>

/**
 * @brief The ConsoleView class
 * This class may be same as SchedulerConsoleView
 */

class ConsoleView : public ConsoleBase
{
    Q_OBJECT
public:
    explicit ConsoleView(QObject *parent = 0);
    ~ConsoleView();

    //ui settings
    void setMultiTask(MultiTask *mltask);
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

#endif // COMMANDLINETEXTEDIT_H
