#ifndef ENTRYTASK_H
#define ENTRYTASK_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QSettings>
#include "../executor/executor.h"

/**
 * @brief The EntryTask class
 *
 * This class is copy of autobatchscheduler/entrytask
 */


class EntryTask : public QObject
{
    Q_OBJECT
public:
    explicit EntryTask(QObject *parent = nullptr);
    ~EntryTask();

    void setFile(QString filepath);
    void setMutex(QMutex *sharedmutex);

    bool getStarted();
    bool getPause();

signals:
    //for progress
    void processInitCount(int start, int end);
    void processCurrent(int currentnum);

    //for progress and menu
    void processError(int currentnum);

    //for textedit
    void processErrorText(QString str);
    void processMessage(QString, int type);

    //for all ui
    void processStarted(int runfrom);
    void processPaused();
    void processStopped();
    void processEnd(int);

public slots:
    //for progress
    void receiveInitCount(int start, int end) { emit processInitCount(start, end); }
    void receiveCurrent(int currentnum){emit processCurrent(currentnum); }

    //for progress and menu
    void receiveError(int currentnum){emit processError(currentnum); }

    //for textedit
    void receiveErrorText(QString str){emit processErrorText(str); }
    void receiveMessage(QString message, int type){emit processMessage(message, type); }
    void sendInput(QString message);

    //for all ui
    void receiveStarted(int runfrom){emit processStarted(runfrom);}
    void receivePaused(){emit processPaused();}
    void receiveStopped(){emit processStopped();}
    void receiveEnd(int type){emit processEnd(type);}

    //from menu
    void start();
    void pause(bool paused);
    void stop();

    //from settest dock
    void updateFileList(QStringList *need); /*int indicates status*/

private slots:
    //debug message
    void processCompleted();

private:
    QMutex *mutex;
    Executor *executor;
    QThread *worker;
//    QString loadfilepath = "";
};

#endif // ENTRYTASK_H
