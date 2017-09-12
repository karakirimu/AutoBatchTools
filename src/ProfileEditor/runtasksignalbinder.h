#ifndef RUNTASKSIGNALBINDER_H
#define RUNTASKSIGNALBINDER_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include "../executor/executor.h"

class RunTaskSignalBinder : public QObject
{
    Q_OBJECT
public:
    explicit RunTaskSignalBinder(QObject *parent = nullptr);
    ~RunTaskSignalBinder();

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
    void processStarted_action(bool);

    void processPaused();
    void processPaused_action(bool);

    void processStopped();
    void processStopped_action(bool);

    void processEnd(int);
    void processEnd_action(bool);

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

    //for all ui
    void receiveStarted(int runfrom){emit processStarted(runfrom); emit processStarted_action(true);}
    void receivePaused(){emit processPaused(); emit processPaused_action(true);}
    void receiveStopped(){emit processStopped(); emit processStopped_action(true);}
    void receiveEnd(int type){emit processEnd(type); emit processEnd_action(true);}

    //from menu
    void start();
    void pause();
    void stop();

    //from settest dock
    void updateRange(QString str);
    void updateFileList(QHash<QString, int> *data); /*int indicates status*/

    void updateEditFile(QString filename, QString editfile);

    //for test
    void testMessage();

private:
    QMutex *mutex;
    Executor *executor;
    QThread *worker;
    QString editingfile;

};

#endif // RUNTASKSIGNALBINDER_H
