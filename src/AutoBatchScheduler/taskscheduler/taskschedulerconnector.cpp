#include "taskschedulerconnector.h"

TaskSchedulerConnector::TaskSchedulerConnector(QObject *parent)
    : TaskScheduler(parent)
{

}

TaskSchedulerConnector::~TaskSchedulerConnector()
{

}

void TaskSchedulerConnector::enableTask(QString objectname, QString filepath)
{
    addTask(objectname, filepath);
//    schedulerStart(objectname);
    emit taskEnabled(objectname);
}

void TaskSchedulerConnector::disableTask(QString objectname)
{
//    schedulerStop(objectname);
    removeTask(objectname);
    emit taskDisabled(objectname);
}
