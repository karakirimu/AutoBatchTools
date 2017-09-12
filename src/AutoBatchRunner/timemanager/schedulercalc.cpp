#include "schedulercalc.h"

SchedulerCalc::SchedulerCalc(QObject *parent) : QObject(parent)
{
    builder = new StartupXmlBuilder();
}

SchedulerCalc::~SchedulerCalc()
{
    delete builder;
}

qint64 SchedulerCalc::getLestSeconds(QString scheduledDateTime)
{
    QDateTime current = QDateTime::currentDateTime();
    QDateTime scheduled = QDateTime::fromString(scheduledDateTime, "yyyy/MM/dd HH:mm:ss");
    return current.secsTo(scheduled);
}

qint64 SchedulerCalc::getLestSeconds(QDateTime scheduledDateTime)
{
    QDateTime current = QDateTime::currentDateTime();
    return current.secsTo(scheduledDateTime);
}

QDateTime SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime)
{
    QDate cdate = QDate::currentDate().addDays(adddays);
    QTime ctime = QTime::fromString(scheduledTime, "HH:mm:ss");
    QDateTime result;
    result.setDate(cdate);
    result.setTime(ctime);

    qDebug() << "SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime) :" << result;
    return result;
}

QDateTime SchedulerCalc::getNextDateTime(qint64 addseconds)
{
    QDateTime result = QDateTime::currentDateTime().addSecs(addseconds);
    qDebug() << "SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime) :" << result;
    return result;
}

QDateTime SchedulerCalc::init_GetScheduledTimeFromXml(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();
    QDateTime time;

    if(builder->readItem(itemid, list)){
        if(list->at(2).at(1) == "yes"){
            switch(VariantConverter::stringToInt(list->at(3).at(1))){
            case 0:
                time = QDateTime::fromString(list->at(4).at(1), "yyyy/MM/dd HH:mm:ss");
                break;
            case 1:
                time = getNextDateTime(getNextDaysCount(list->at(7).at(1), list->at(6).at(1))
                                       , list->at(6).at(1));
                break;
            case 2:
                time = QDateTime::currentDateTime().addSecs(VariantConverter::VariantConverter::stringToInt(list->at(5).at(1)));
                break;
            default:
                break;
            }
        }
    }
    qDebug() << "select num" << list->at(3).at(1);
    qDebug() << "SchedulerCalc::init_GetScheduledTimeFromXml(int itemid) : " << time;
    return time;
}

QDateTime SchedulerCalc::getNextTimeFromXml(int itemid, QDateTime current)
{
    QList<QStringList> *list = new QList<QStringList>();
    QDateTime time;

    if(builder->readItem(itemid, list)){
        if(list->at(2).at(1) == "yes" && VariantConverter::stringToInt(list->at(3).at(1)) == 2){
            time = current.addSecs(VariantConverter::stringToInt(list->at(5).at(1)));
        }
        if(list->at(2).at(1) == "yes" && VariantConverter::stringToInt(list->at(3).at(1)) == 1){
            time = getNextDateTime(getNextDaysCount(list->at(7).at(1), list->at(6).at(1))
                                   , list->at(6).at(1));
        }
    }

    qDebug() << "SchedulerCalc::getNextTimeFromXml(int itemid) : " << time;
    return time;
}

int SchedulerCalc::currentDayOfTheWeek()
{
    return QDate::currentDate().dayOfWeek();
}

int SchedulerCalc::getNextDaysCount(QString datecode, QString timedata)
{
    int result = 0;
    int dayofweek = currentDayOfTheWeek();
    bool throughmax = false;
    QString charcode;

    //check current day of the week is on

    //other day scheduled (start from next day)
    for(int i = dayofweek + 1; i < WEEKDATA; i++){
        result++;
        //breakcode
        charcode = datecode.at(i);
        if(charcode.toInt() == 1) break;

        //reset i (if maximum number)
        if(i == (WEEKDATA - 1)){
            throughmax = true;
            i = 1;
        }
    }

    //add weeks
    if(throughmax){
        result += (VariantConverter::stringToInt(datecode.at(0)) - 1) * 7;
    }

    //current day scheduled (first)
    if(result % 7 == 0){
        //this daycount is 7 or 0
        QDate cdate = QDate::currentDate();
        QTime ctime = QTime::fromString(timedata, "HH:mm:ss");
        QDateTime rtime;
        rtime.setDate(cdate);
        rtime.setTime(ctime);

        //if expired today
        if(getLestSeconds(rtime) > 0){
            // 0 days
            result = 0;
        }
    }

    return result;
}

int SchedulerCalc::dateCheckedCounter(QString datecode)
{
    int count = 0;
    for(int i = 1; i < WEEKDATA; i++){
        if(VariantConverter::stringToInt(datecode.at(i)) == 1) count++;
    }
    return count;
}
