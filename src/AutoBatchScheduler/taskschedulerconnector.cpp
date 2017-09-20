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
    emit taskEnabled(objectname);
    schedulerStart(objectname);
}

void TaskSchedulerConnector::disableTask(QString objectname)
{
    schedulerStop(objectname);
    emit taskDisabled(objectname);
}
