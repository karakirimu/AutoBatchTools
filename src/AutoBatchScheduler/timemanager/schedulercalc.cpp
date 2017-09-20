#include "schedulercalc.h"

SchedulerCalc::SchedulerCalc(QObject *parent) : QObject(parent)
{
    builder = new StartupXmlBuilder();
    connect(this, &SchedulerCalc::loop, this, &SchedulerCalc::start);
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

int SchedulerCalc::getStartupXmlIndex(QString objectname)
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

int SchedulerCalc::getSelectedxmlindex() const
{
    return selectedxmlindex;
}

void SchedulerCalc::setSelectedxmlindex(QString objectname)
{
    selectedxmlindex = getStartupXmlIndex(objectname);
}

QDateTime SchedulerCalc::getSchedate() const
{
    return schedate;
}

bool SchedulerCalc::getRunning() const
{
    return running;
}

qint64 SchedulerCalc::getRefreshms() const
{
    return refreshms;
}

void SchedulerCalc::setRefreshms(const qint64 &value)
{
    refreshms = value;
}

void SchedulerCalc::setMutex(QMutex *value)
{
    mutex = value;
}

QDateTime SchedulerCalc::getNextDateTime(qint64 addseconds)
{
    QDateTime result = QDateTime::currentDateTime().addSecs(addseconds);
    qDebug() << "SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime) :" << result;
    return result;
}

//DEPENDS_XML
QDateTime SchedulerCalc::init_GetScheduledTimeFromXml(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();
    QDateTime time;

    if(builder->readItem(itemid, list)){
        if(list->at(2).at(1) == "yes"){
            switch(VariantConverter::stringToInt(list->at(3).at(1))){
            case 0:
                time = QDateTime::fromString(list->at(4).at(1), "yyyy/MM/dd HH:mm:ss");
                loopschedule = false;
                break;
            case 1:
                time = getNextDateTime(getNextDaysCount(list->at(7).at(1), list->at(6).at(1))
                                       , list->at(6).at(1));
                loopschedule = true;
                break;
            case 2:
                time = QDateTime::currentDateTime().addSecs(VariantConverter::VariantConverter::stringToInt(list->at(5).at(1)));
                loopschedule = true;
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

    delete list;
    qDebug() << "SchedulerCalc::getNextTimeFromXml(int itemid) : " << time;
    return time;
}

int SchedulerCalc::currentDayOfTheWeek()
{
    return QDate::currentDate().dayOfWeek();
}

void SchedulerCalc::start()
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

void SchedulerCalc::stop()
{
    running = false;
}

void SchedulerCalc::loopstop()
{
    loopschedule = false;
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
