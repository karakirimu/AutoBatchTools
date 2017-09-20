#include "entrytask.h"

EntryTask::EntryTask(QObject *parent)
    : QObject(parent)
{
    //set internal thread
    worker = new QThread();

    //setmain worker
    executor = new Executor();

    //init executor const
    executor->setLaunchedfrom(Executor::SCHEDULER);

    //connect thread to doWork
    connect(worker, &QThread::started, executor, &Executor::runProcess);
    connect(executor, &Executor::processEnded, worker, &QThread::quit);
    connect(executor, &Executor::processStopped, worker, &QThread::quit);
    connect(worker, &QThread::finished, this, &EntryTask::processCompleted);

    //move to thread
    executor->moveToThread(worker);

    //connect executor
    connect(executor, &Executor::processStarted, this, &EntryTask::receiveStarted);
    connect(executor, &Executor::processEnded, this, &EntryTask::receiveEnd);
    connect(executor, &Executor::processStopped, this, &EntryTask::receiveStopped);
    connect(executor, &Executor::processPaused, this, &EntryTask::receivePaused);

    connect(executor, &Executor::processStateUpdate, this, &EntryTask::receiveCurrent);
    connect(executor, &Executor::processStateCount, this, &EntryTask::receiveInitCount);
    connect(executor, &Executor::processCheckError, this, &EntryTask::receiveErrorText);
    connect(executor, &Executor::processMessage, this, &EntryTask::receiveMessage);

}

EntryTask::~EntryTask()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait();
    }
    delete worker;
    delete executor;
}

void EntryTask::setFile(QString filepath)
{
    loadfilepath = filepath;
}

bool EntryTask::getStarted()
{
    return executor->getWorking();
}

bool EntryTask::getPause()
{
    return executor->getPaused();
}

void EntryTask::start()
{
    if(executor->getPaused()){
        executor->setPaused(false);

    }else{
        //TODO:
        //load static settings
//        QSettings settings( "./psettings.ini", QSettings::IniFormat );

//        settings.beginGroup("TESTEXEC");
//        executor->setDetached(settings.value("DETACH", false).toBool());
//        executor->setLaunchedfrom(settings.value("FAKERES", false).toInt());
//        executor->setSearchfileoverwrite(settings.value("FSUPDATE", true).toBool());
//        executor->setForcequittime(settings.value("TIMEOUT", 30000).toInt());
//        settings.endGroup();
        executor->setProcessFile(loadfilepath);

        executor->setGlobalList();
        executor->setLocalList();

        worker->start();
    }
}

void EntryTask::pause(bool value)
{
    executor->setPaused(value);
}

void EntryTask::stop()
{
    executor->stopProcess();
}

void EntryTask::updateFileList(QHash<QString, int> *data)
{
    if(data->empty()) return;
    QStringList need;

    QHash<QString, int>::iterator i = data->begin();
    while (i != data->end() && i.value() == 0) {
        need.append(i.key());
        ++i;
    }

//    QSettings settings( "./psettings.ini", QSettings::IniFormat );
//    settings.beginGroup("TESTEXEC");
//    executor->addInputFiles(need, settings.value("FILELOADMAX", 1).toInt());
    //    settings.endGroup();
}

void EntryTask::processCompleted()
{
    qDebug() << "EntryTask::processComplete";
}
