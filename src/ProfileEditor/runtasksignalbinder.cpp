#include "runtasksignalbinder.h"

#include <QSettings>

RunTaskSignalBinder::RunTaskSignalBinder(QObject *parent)
    : QObject(parent)
{
    //set internal thread
    worker = new QThread();

    //setmain worker
    executor = new Executor();

    //connect thread to doWork
    connect(worker, &QThread::started, executor, &Executor::runProcess);
    connect(executor, &Executor::processEnded, worker, &QThread::quit);
    connect(executor, &Executor::processStopped, worker, &QThread::quit);
    connect(worker, &QThread::finished, this, &RunTaskSignalBinder::testMessage);

    //move to thread
    executor->moveToThread(worker);

    //connect executor
    connect(executor, &Executor::processStarted, this, &RunTaskSignalBinder::receiveStarted);
    connect(executor, &Executor::processEnded, this, &RunTaskSignalBinder::receiveEnd);
    connect(executor, &Executor::processStopped, this, &RunTaskSignalBinder::receiveStopped);
    connect(executor, &Executor::processPaused, this, &RunTaskSignalBinder::receivePaused);

    connect(executor, &Executor::processStateUpdate, this, &RunTaskSignalBinder::receiveCurrent);
    connect(executor, &Executor::processStateCount, this, &RunTaskSignalBinder::receiveInitCount);
    connect(executor, &Executor::processCheckError, this, &RunTaskSignalBinder::receiveErrorText);
    connect(executor, &Executor::processMessage, this, &RunTaskSignalBinder::receiveMessage);
}

RunTaskSignalBinder::~RunTaskSignalBinder()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait();
    }
    delete worker;
    delete executor;
}

void RunTaskSignalBinder::start()
{
    if(executor->getPaused()){
        executor->setPaused(false);

    }else{
        //load static settings
        QSettings settings( "./psettings.ini", QSettings::IniFormat );

        settings.beginGroup("TESTEXEC");
        executor->setDetached(settings.value("DETACH", false).toBool());
        executor->setLaunchedfrom(settings.value("FAKERES", false).toInt());
        executor->setSearchfileoverwrite(settings.value("FSUPDATE", true).toBool());
//        executor->setForcequittime(settings.value("TIMEOUT", 30000).toInt());
        settings.endGroup();

        executor->setProcessFile(editingfile);
        executor->setGlobalList();
        executor->setLocalList();

        worker->start();
    }
}

void RunTaskSignalBinder::pause()
{
    executor->setPaused(true);
}

void RunTaskSignalBinder::stop()
{
    executor->stopProcess();
}

void RunTaskSignalBinder::updateRange(QString str)
{
    QStringList split = str.split(",");
    QStringList tmp;
    int tmpstart = 0;
    int tmpend = 0;
    QList<int> result;
    foreach(QString var, split){
        if(var.contains("-")){
            tmp.clear();
            tmp = var.split("-");

            tmpstart = ((QString)tmp.at(0)).toInt();
            tmpend = ((QString)tmp.at(1)).toInt();

            for(; tmpstart < tmpend; tmpstart++){
                result.append(tmpstart > 0 ? tmpstart + 1 : 0);
            }

        }else{
            result.append(var.toInt() > 0 ? var.toInt() + 1 : 0);
        }
    }

    executor->setExecList(result);
}

void RunTaskSignalBinder::updateFileList(QHash<QString, int> *data)
{
    if(data->empty()) return;
    QStringList need;

    QHash<QString, int>::iterator i = data->begin();
    while (i != data->end() && i.value() == 0) {
        need.append(i.key());
        ++i;
    }

    QSettings settings( "./psettings.ini", QSettings::IniFormat );
    settings.beginGroup("TESTEXEC");
    executor->addInputFiles(need, settings.value("FILELOADMAX", 1).toInt());
    settings.endGroup();
}

void RunTaskSignalBinder::updateEditFile(QString filename, QString editfile)
{
    Q_UNUSED(filename)
    editingfile = editfile;
}

void RunTaskSignalBinder::testMessage()
{
    qDebug() << "process completed";
}
