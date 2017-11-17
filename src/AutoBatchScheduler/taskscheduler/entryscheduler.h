#ifndef TIMESCHEDULERTHREAD_H
#define TIMESCHEDULERTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <schedulerwait.h>
#include <startupxmlbuilder.h>

class EntryScheduler : public QObject
{
    Q_OBJECT
public:
    explicit EntryScheduler(QObject *parent = 0);
    ~EntryScheduler();

    bool isStarted();
    void setRefreshTime(int sec); //preset
//    void setXmlItemId(int itemid); //preset
    void setMutex(QMutex *mutex); //preset

signals:
    void timerStarted(QDateTime);
    void timerFinished(int);
    void encounteredScheduledTime();

public slots:
    void start();
    void stop();

    //receive
    void receiveStarted(QDateTime datetime){emit timerStarted(datetime);}
    void receiveFinished(int value){emit timerFinished(value);}
    void receiveEncounter(){emit encounteredScheduledTime();}

    void setXmlItemId(QString objname);

private:
    SchedulerWait *swait;
    QThread *worker;
};

#endif // TIMESCHEDULERTHREAD_H
