#ifndef MULTITASK_H
#define MULTITASK_H

#include <QObject>
#include <QTime>
#include <entrytask.h>

class MultiTask : public QObject
{
    Q_OBJECT
public:
    explicit MultiTask(QObject *parent = nullptr);
    ~MultiTask();

    bool taskRunningCheck(QString objectname);

    void addTask(QString objectname, QString processfile);
    void removeTask(QString objectname);

    void setInputFileList(QString objectname, QStringList *list);

    QString generateRandom(int length);
signals:
    //from EntryTask to ui-------------------------------------------------
//  For multitask ui manage function
//    void processInitCount(QString objectname, int start, int end);
//    void processCurrent(QString objectname, int currentnum);
    void processInitCount(int start, int end);
    void processCurrent(int currentnum);

    void processError(QString objectname, int currentnum);
    void processErrorText(QString objectname, QString str);
    void processMessage(QString objectname, QString, int type);

    void processStarted(QString objectname, int runfrom);
    void processPaused(QString objectname);
    void processStopped(QString objectname);
    void processEnd(QString objectname, int);

public slots:
    //from ui class
    void processPause(QString objectname);
    void processStop(QString objectname);
    void sendInput(QString objectname, QString text);

    //from EntryTask-----------------------------------------------------------
// For multitask ui manage. Currently it does singletask.
//    void receiveInitCount(int start, int end) { emit processInitCount(sender()->objectName(), start, end); }
//    void receiveCurrent(int currentnum){emit processCurrent(sender()->objectName(), currentnum); }
    void receiveInitCount(int start, int end) { emit processInitCount(start, end); }
    void receiveCurrent(int currentnum){emit processCurrent(currentnum); }

    void receiveError(int currentnum){emit processError(sender()->objectName(), currentnum); }
    void receiveErrorText(QString str){emit processErrorText(sender()->objectName(), str); }
    void receiveMessage(QString message, int type){emit processMessage(sender()->objectName(), message, type); }

    void receiveStarted(int runfrom){emit processStarted(sender()->objectName(), runfrom); /*emit processStarted_action(true);*/}
    void receivePaused(){emit processPaused(sender()->objectName()); /*emit processPaused_action(true);*/}
    void receiveStopped(){emit processStopped(sender()->objectName()); /*emit processStopped_action(true);*/}
    void receiveEnd(int type){emit processEnd(sender()->objectName(), type); /*emit processEnd_action(true);*/}

private:
    bool processAliveCheck(QString objectname);

    QHash<QString, EntryTask *> *task;
    QMutex *basemutex;
};

#endif // MULTITASK_H
