#ifndef TASKSCHEDULERCONNECTOR_H
#define TASKSCHEDULERCONNECTOR_H

#include "taskscheduler.h"

class TaskSchedulerConnector : public TaskScheduler
{
    Q_OBJECT
public:
    explicit TaskSchedulerConnector(QObject *parent = nullptr);
    ~TaskSchedulerConnector();

signals:
    void taskDisabled(QString);
    void taskEnabled(QString);

    //for syncronize table
    //startuptable <-> processshowtable
    void tableInserted(int);
    void tableDeleted(int);
    void tableReplaced(int);
    void tableEnabled(QString);
    void tableDisabled(QString);

public slots:
    //from systemtray or settingdialog
    void enableTask(QString objectname, QString filepath); //set objectname
    void disableTask(QString objectname);

    //from startuptable
//    void actionAdded(int itemid){emit addListAction(itemid);}
//    void actionDeleted(QString objectname){emit deleteListAction(objectname);}

};

#endif // TASKSCHEDULERCONNECTOR_H
