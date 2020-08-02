#ifndef RUNTASKSIGNALBINDER_H
#define RUNTASKSIGNALBINDER_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include "../executor/executor.h"
#include "settingconstant.h"

class RunTaskSignalBinder : public QObject
{
    Q_OBJECT
public:
    explicit RunTaskSignalBinder(QObject *parent = nullptr);
    ~RunTaskSignalBinder();

    bool getStarted();
    bool getPause();
    void setMutex(QMutex *sharedmutex);
    void setFile(QString filepath);

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
    //for executor signals
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
    void pause(bool pause);
    void stop();

    //from settest dock
    void updateRange(QString str);
    void updateFileList(QStringList *data); /*int indicates status*/

private:
    QMutex *mutex;
    Executor *executor;
    QThread *worker;

};

#endif // RUNTASKSIGNALBINDER_H
