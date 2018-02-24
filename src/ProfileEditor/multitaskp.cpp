#include "multitask.h"

MultiTask::MultiTask(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, RunTaskSignalBinder *>();
    basemutex = new QMutex();

    //time based seed set
    QTime *ti = new QTime();
    qsrand(ti->currentTime().msecsSinceStartOfDay() ^ 123456789);
    delete ti;
}

MultiTask::~MultiTask()
{
    //delete all left tasks
    QHash<QString, RunTaskSignalBinder *>::iterator i = task->begin();
    while (i != task->end()) {
        removeTask(i.key());
        ++i;
    }

    delete task;
    delete basemutex;
}

bool MultiTask::taskRunningCheck(QString objectname)
{
    bool result = false;
    if(processAliveCheck(objectname)){
        RunTaskSignalBinder *et = task->value(objectname);
        result = et->getStarted();
    }

    return result;
}

void MultiTask::addTask(QString objectname, QString processfile)
{
    //new task set
    RunTaskSignalBinder *et = new RunTaskSignalBinder();

    //connect message
    connect(et, &RunTaskSignalBinder::processInitCount, this, &MultiTask::receiveInitCount);
    connect(et, &RunTaskSignalBinder::processCurrent, this, &MultiTask::receiveCurrent);
    connect(et, &RunTaskSignalBinder::processError, this, &MultiTask::receiveError);
    connect(et, &RunTaskSignalBinder::processErrorText, this, &MultiTask::receiveErrorText);
    connect(et, &RunTaskSignalBinder::processMessage, this, &MultiTask::receiveMessage);
    connect(et, &RunTaskSignalBinder::processStarted, this, &MultiTask::receiveStarted);
    connect(et, &RunTaskSignalBinder::processPaused, this, &MultiTask::receivePaused);
    connect(et, &RunTaskSignalBinder::processStopped, this, &MultiTask::receiveStopped);
    connect(et, &RunTaskSignalBinder::processEnd, this, &MultiTask::receiveEnd);

    //set task filename
    et->setFile(processfile);

    //set same objectname
    et->setObjectName(objectname);

    //set shared mutex (for secure running)
    et->setMutex(basemutex);

    //add pointer
    task->insert(objectname, et);

    //start task
    et->start();
}

void MultiTask::removeTask(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    //stop process
    et->stop();

    //disconnect child object
    disconnect(et, &RunTaskSignalBinder::processInitCount, this, &MultiTask::receiveInitCount);
    disconnect(et, &RunTaskSignalBinder::processCurrent, this, &MultiTask::receiveCurrent);
    disconnect(et, &RunTaskSignalBinder::processError, this, &MultiTask::receiveError);
    disconnect(et, &RunTaskSignalBinder::processErrorText, this, &MultiTask::receiveErrorText);
    disconnect(et, &RunTaskSignalBinder::processMessage, this, &MultiTask::receiveMessage);
    disconnect(et, &RunTaskSignalBinder::processStarted, this, &MultiTask::receiveStarted);
    disconnect(et, &RunTaskSignalBinder::processPaused, this, &MultiTask::receivePaused);
    disconnect(et, &RunTaskSignalBinder::processStopped, this, &MultiTask::receiveStopped);
    disconnect(et, &RunTaskSignalBinder::processEnd, this, &MultiTask::receiveEnd);

    //delete register
    task->remove(objectname);

    delete et;
}

void MultiTask::setInputFileList(QString objectname, QStringList *list)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    et->updateFileList(list);
}

void MultiTask::setRange(QString objectname, QString str)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    et->updateRange(str);
}

void MultiTask::processPause(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    if(et->getStarted() && !et->getPause()){
        et->pause(true);
    }else{
        et->pause(false);
    }
}

void MultiTask::processStop(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(et->getStarted()) et->stop();
}

void MultiTask::sendInput(QString objectname, QString text)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(et->getStarted()) et->sendInput(text);
}

bool MultiTask::processAliveCheck(QString objectname)
{
    return task->contains(objectname) ? true : false;
}

QString MultiTask::generateRandom(int length)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.");

    QString randomString;
    for(int i=0; i < length; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
