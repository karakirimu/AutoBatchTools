#ifndef SCHEDULERCALC_H
#define SCHEDULERCALC_H

#include <../variantconverter/variantconverter.h>
#include <QObject>
#include <QMutex>
#include <QThread>
#include <QDateTime>
#include "startupxmlbuilder.h"

#define WEEKDATA 8

class SchedulerCalc : public QObject
{
    Q_OBJECT
public:
    explicit SchedulerCalc(QObject *parent = nullptr);
    ~SchedulerCalc();

    void setMutex(QMutex *value); //preset

    qint64 getRefreshms() const;
    void setRefreshms(const qint64 &value);//preset

    bool getRunning() const;

    QDateTime getSchedate() const;

    int getSelectedxmlindex() const;
    void setSelectedxmlindex(QString objectname);//preset

    enum{FINISHED,EXPIRED};
signals:
    //send to ui
    void timerStarted(QDateTime);

    //below functions emitted, deleted this task
    void timerFinished(int);

    void encounterScheduledTime();

    //internal signal
    void loop();
public slots:
    //for timer
    void start();
    void stop();
    void loopstop();

private:
    qint64 getLestSeconds(QString scheduledDateTime);
    qint64 getLestSeconds(QDateTime scheduledDateTime);
    QDateTime getNextDateTime(qint64 addseconds);
    QDateTime init_GetScheduledTimeFromXml(int itemid);
    QDateTime getNextTimeFromXml(int itemid, QDateTime current);
    int currentDayOfTheWeek();

    //inner calc functions
    int getNextDaysCount(QString datecode, QString timedata);
    int dateCheckedCounter(QString datecode);
    QDateTime getNextDateTime(qint64 adddays, QString scheduledTime);

    int getStartupXmlIndex(QString objectname);

    StartupXmlBuilder *builder;

    //variant for manage
    qint64 refreshms = 1000;
    int selectedxmlindex = -1;
    QDateTime schedate;
    QMutex *mutex;

    bool running = false;

    //when schedule loop
    bool loopschedule = false;
};

#endif // SCHEDULERCALC_H
