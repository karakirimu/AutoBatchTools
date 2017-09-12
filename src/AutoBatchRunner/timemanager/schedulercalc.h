#ifndef SCHEDULERCALC_H
#define SCHEDULERCALC_H

#include <startupxmlbuilder.h>
#include <../variantconverter/variantconverter.h>

#include <QObject>
#include <QDateTime>

class SchedulerCalc : public QObject
{
    Q_OBJECT
public:
    explicit SchedulerCalc(QObject *parent = 0);
    ~SchedulerCalc();

    qint64 getLestSeconds(QString scheduledDateTime);
    qint64 getLestSeconds(QDateTime scheduledDateTime);
    QDateTime getNextDateTime(qint64 addseconds);
    QDateTime init_GetScheduledTimeFromXml(int itemid);
    QDateTime getNextTimeFromXml(int itemid, QDateTime current);
    int currentDayOfTheWeek();

private:
    int getNextDaysCount(QString datecode, QString timedata);
    int dateCheckedCounter(QString datecode);
    QDateTime getNextDateTime(qint64 adddays, QString scheduledTime);
    StartupXmlBuilder *builder;
    const int WEEKDATA = 8;
};

#endif // SCHEDULERCALC_H
