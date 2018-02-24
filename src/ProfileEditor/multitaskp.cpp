#include "multitaskp.h"

MultiTaskP::MultiTaskP(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, RunTaskSignalBinder *>();
    basemutex = new QMutex();

    //time based seed set
    QTime *ti = new QTime();
    qsrand(ti->currentTime().msecsSinceStartOfDay() ^ 123456789);
    delete ti;
}

MultiTaskP::~MultiTaskP()
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

bool MultiTaskP::taskRunningCheck(QString objectname)
{
    bool result = false;
    if(processAliveCheck(objectname)){
        RunTaskSignalBinder *et = task->value(objectname);
        result = et->getStarted();
    }

    return result;
}

void MultiTaskP::addTask(QString objectname, QString processfile)
{
    //new task set
    RunTaskSignalBinder *et = new RunTaskSignalBinder();

    //connect message
    connect(et, &RunTaskSignalBinder::processInitCount, this, &MultiTaskP::receiveInitCount);
    connect(et, &RunTaskSignalBinder::processCurrent, this, &MultiTaskP::receiveCurrent);
    connect(et, &RunTaskSignalBinder::processError, this, &MultiTaskP::receiveError);
    connect(et, &RunTaskSignalBinder::processErrorText, this, &MultiTaskP::receiveErrorText);
    connect(et, &RunTaskSignalBinder::processMessage, this, &MultiTaskP::receiveMessage);
    connect(et, &RunTaskSignalBinder::processStarted, this, &MultiTaskP::receiveStarted);
    connect(et, &RunTaskSignalBinder::processPaused, this, &MultiTaskP::receivePaused);
    connect(et, &RunTaskSignalBinder::processStopped, this, &MultiTaskP::receiveStopped);
    connect(et, &RunTaskSignalBinder::processEnd, this, &MultiTaskP::receiveEnd);

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

void MultiTaskP::removeTask(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    //stop process
    et->stop();

    //disconnect child object
    disconnect(et, &RunTaskSignalBinder::processInitCount, this, &MultiTaskP::receiveInitCount);
    disconnect(et, &RunTaskSignalBinder::processCurrent, this, &MultiTaskP::receiveCurrent);
    disconnect(et, &RunTaskSignalBinder::processError, this, &MultiTaskP::receiveError);
    disconnect(et, &RunTaskSignalBinder::processErrorText, this, &MultiTaskP::receiveErrorText);
    disconnect(et, &RunTaskSignalBinder::processMessage, this, &MultiTaskP::receiveMessage);
    disconnect(et, &RunTaskSignalBinder::processStarted, this, &MultiTaskP::receiveStarted);
    disconnect(et, &RunTaskSignalBinder::processPaused, this, &MultiTaskP::receivePaused);
    disconnect(et, &RunTaskSignalBinder::processStopped, this, &MultiTaskP::receiveStopped);
    disconnect(et, &RunTaskSignalBinder::processEnd, this, &MultiTaskP::receiveEnd);

    //delete register
    task->remove(objectname);

    delete et;
}

void MultiTaskP::setInputFileList(QString objectname, QStringList *list)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    et->updateFileList(list);
}

void MultiTaskP::setRange(QString objectname, QString str)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    et->updateRange(str);
}

void MultiTaskP::processPause(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    if(et->getStarted() && !et->getPause()){
        et->pause(true);
    }else{
        et->pause(false);
    }
}

void MultiTaskP::processStop(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(et->getStarted()) et->stop();
}

void MultiTaskP::sendInput(QString objectname, QString text)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(et->getStarted()) et->sendInput(text);
}

bool MultiTaskP::processAliveCheck(QString objectname)
{
    return task->contains(objectname) ? true : false;
}

QString MultiTaskP::generateRandom(int length)
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
