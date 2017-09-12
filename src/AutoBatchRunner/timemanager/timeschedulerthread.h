#ifndef TIMESCHEDULERTHREAD_H
#define TIMESCHEDULERTHREAD_H

#include <QObject>
#include <QThread>
#include <schedulercalc.h>
#include <startupxmlbuilder.h>

class TimeSchedulerThread : public QObject
{
    Q_OBJECT
public:
    explicit TimeSchedulerThread(QObject *parent = 0);
    ~TimeSchedulerThread();

    bool isStarted();

    void setMovedThread(QThread *thread);
    void setThreadName(QString name);
    void setRefreshTime(int sec);
    void setScheduledTime(QDateTime scheduled);
    void setXmlItemId(int itemid);
    void forceStop();
    void stop();

    QString getName();
    QDateTime getScheduledTime();
    int getXmlId();
    QThread *getMovedThread();
signals:
    void timerStarted(QString);
    void timerStopped(QString);
    void timerAlreadyTimeout(QString);
    void encounteredTimeout(QString);

public slots:
    void start();

private slots:
    void doWork();

private:
    SchedulerCalc *scalc;
    StartupXmlBuilder *builder;

    bool running;
    int xmlitemid;
    QString name;
    qint64 refreshtime;
    QThread *workerthread;
    QDateTime scheduled;
};

#endif // TIMESCHEDULERTHREAD_H
