#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QObject>
#include <timeschedulerthread.h>

class TimeManager : public QObject
{
    Q_OBJECT
public:
    explicit TimeManager(QObject *parent = 0);
    ~TimeManager();

    QString addTimer(int itemid);
    void removeTimer(QString name);
    QDateTime getScheduledTime(QString threadname);
    void count();
signals:
    void timerStarted(QString);
    void timerStopped(QString);
    void timerAlreadyExpired(QString);
    void timerExpired(QString);

public slots:
    void threadStarted(QString name);
    void threadStopped(QString name);
    void loadExec(QString name);    
    void threadObsoluted(QString name);

private:
    void reloadTimer(TimeSchedulerThread *old);

    QString getRandomString(int size);
    QList<TimeSchedulerThread*> *timerlist;
    SchedulerCalc *scalc;
};

#endif // TIMEMANAGER_H
