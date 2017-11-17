#include "schedulerwait.h"

SchedulerWait::SchedulerWait(QObject *parent) : QObject(parent)
{
    builder = new StartupXmlBuilder();
    connect(this, &SchedulerWait::loop, this, &SchedulerWait::start);
}

SchedulerWait::~SchedulerWait()
{
    delete builder;
}

int SchedulerWait::getSelectedxmlindex() const
{
    return selectedxmlindex;
}

void SchedulerWait::setSelectedxmlindex(QString objectname)
{
    selectedxmlindex = getStartupXmlIndex(objectname);
}

QDateTime SchedulerWait::getSchedate() const
{
    return schedate;
}

bool SchedulerWait::getRunning() const
{
    return running;
}

qint64 SchedulerWait::getRefreshms() const
{
    return refreshms;
}

void SchedulerWait::setRefreshms(const qint64 &value)
{
    refreshms = value;
}

void SchedulerWait::setMutex(QMutex *value)
{
    mutex = value;
}

qint64 SchedulerWait::getLestSeconds(QString scheduledDateTime)
{
    QDateTime scheduled = QDateTime::fromString(scheduledDateTime, "yyyy/MM/dd HH:mm:ss");
    return QDateTime::currentDateTime().secsTo(scheduled);
}

qint64 SchedulerWait::getLestSeconds(QDateTime scheduledDateTime)
{
    return QDateTime::currentDateTime().secsTo(scheduledDateTime);
}

QDateTime SchedulerWait::getNextDateTime(qint64 adddays, QString scheduledTime)
{
    QDate cdate = QDate::currentDate().addDays(adddays);
    QTime ctime = QTime::fromString(scheduledTime, "HH:mm:ss");
    QDateTime result;
    result.setDate(cdate);
    result.setTime(ctime);

    qDebug() << "SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime) :" << result;
    return result;
}

QDateTime SchedulerWait::getNextDateTime(qint64 addseconds)
{
    QDateTime result = QDateTime::currentDateTime().addSecs(addseconds);
    qDebug() << "SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime) :" << result;
    return result;
}

//DEPENDS_XML
QDateTime SchedulerWait::init_GetScheduledTimeFromXml(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();
    QDateTime time;

    if(builder->readItem(itemid, list)){
        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"){
            switch(VariantConverter::stringToInt(list->at(StartupXmlBuilder::SELECTEDTYPE).at(1))){
            case StartupXmlBuilder::ONESHOT:
                time = QDateTime::fromString(list->at(StartupXmlBuilder::SC_DATETIME).at(1)
                                            , "yyyy/MM/dd HH:mm:ss");
                loopschedule = false;
                break;

            case StartupXmlBuilder::WEEKLOOP:
                time = getNextDateTime(getNextDaysCount(list->at(StartupXmlBuilder::SC_DAY).at(1)
                                                        , list->at(StartupXmlBuilder::SC_TIME).at(1))
                                                        , list->at(StartupXmlBuilder::SC_TIME).at(1));
                loopschedule = true;
                break;

            case StartupXmlBuilder::TIMELOOP:
                time = QDateTime::currentDateTime().addSecs(VariantConverter::VariantConverter::stringToInt(list->at(5).at(1)));
                loopschedule = true;
                break;

            default:
                break;
            }
        }
    }
    qDebug() << "select num" << list->at(StartupXmlBuilder::SELECTEDTYPE).at(1);
    qDebug() << "SchedulerCalc::init_GetScheduledTimeFromXml(int itemid) : " << time;
    return time;
}

QDateTime SchedulerWait::getNextTimeFromXml(int itemid, QDateTime current)
{
    QList<QStringList> *list = new QList<QStringList>();
    QDateTime time;

    if(builder->readItem(itemid, list)){
        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"
                && VariantConverter::stringToInt(list->at(3).at(1)) == 2){
            time = current.addSecs(VariantConverter::stringToInt(list->at(5).at(1)));
        }
        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"
                && VariantConverter::stringToInt(list->at(3).at(1)) == 1){
            time = getNextDateTime(getNextDaysCount(list->at(7).at(1), list->at(6).at(1))
                                   , list->at(6).at(1));
        }
    }

    delete list;
    qDebug() << "SchedulerCalc::getNextTimeFromXml(int itemid) : " << time;
    return time;
}

int SchedulerWait::currentDayOfTheWeek()
{
    return QDate::currentDate().dayOfWeek();
}

int SchedulerWait::getStartupXmlIndex(QString objectname)
{
    //Copy function. from SystemTray
    QList<QStringList> *list = new QList<QStringList>();

    //search valid data
    int itemid = -1;
    int count = builder->count();

    for(int i = 0; i < count; i++){
        list->clear();
        if(builder->readItem(i, list)
                && objectname == list->at(StartupXmlBuilder::UNIQUE).at(1)){
            itemid = i;
            break;
        }
    }

    delete list;
    return itemid;
}

void SchedulerWait::start()
{
    qint64 lesttime;
    //check null
    if(selectedxmlindex == -1) return;

    QDateTime scheduledDateTime;

    //instruct schedule
//    if(schedate.isValid()){
//        scheduledDateTime = schedate;
//    }else{
    mutex->lock();
    scheduledDateTime = init_GetScheduledTimeFromXml(selectedxmlindex);
    schedate = scheduledDateTime;
    mutex->unlock();
//    }

    qDebug() << scheduledDateTime << "TimeSchedulerThread::start()";

    //stop timer when first time expired.
    if(getLestSeconds(scheduledDateTime) < 0){
        mutex->lock();
        qDebug() << "TimeSchedulerThread::start() : timerStopped()";
        emit timerFinished(EXPIRED);
        mutex->unlock();
        return;
    }

    //for restarting
    running = true;

    emit timerStarted(scheduledDateTime);

    while(running){
        lesttime = getLestSeconds(scheduledDateTime);
        if(lesttime > 0){
            QThread::msleep(refreshms);
        }else{
            mutex->lock();
            emit encounterScheduledTime();
            running = false;
            mutex->unlock();
        }
    }

    //loop command
    if(loopschedule){
        emit loop();
    }else{
        emit timerFinished(FINISHED);
    }
}

void SchedulerWait::stop()
{
    running = false;
}

void SchedulerWait::loopstop()
{
    loopschedule = false;
}

int SchedulerWait::getNextDaysCount(QString datecode, QString timedata)
{
    int result = 0;
    int dayofweek = currentDayOfTheWeek();
    int i = dayofweek;
    int loop = 0;
    bool next = true;
    QString charcode;

    while(next){
        loop = i - ((i-1)/7)*7;
        charcode = datecode.at(loop);
        if(charcode.toInt() == 1){
            if(i == dayofweek){
                //current day scheduled
                //this daycount is 7 or 0
                QDateTime rtime;
                rtime.setDate(QDate::currentDate());
                rtime.setTime(QTime::fromString(timedata, "HH:mm:ss"));

                //if not expired today
                if(getLestSeconds(rtime) > 0) next = false;
            }else{
                //other day scheduled
                result = i - dayofweek;
                next = false;
            }
        }
        i++;
    }

    //add weeks
    result += (VariantConverter::stringToInt(datecode.at(0)) - 1) * 7;

    return result;
}

//int SchedulerCalc::dateCheckedCounter(QString datecode)
//{
//    int count = 0;
//    for(int i = 1; i < WEEKDATA; i++){
//        if(VariantConverter::stringToInt(datecode.at(i)) == 1) count++;
//    }
//    return count;
//}
