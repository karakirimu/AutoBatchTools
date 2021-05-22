/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include "entrytask.h"

#include <QObject>
#include <entryscheduler.h>


//task binder class with no gui signals or slots

class TaskScheduler : public QObject
{
    Q_OBJECT
public:
    explicit TaskScheduler(QObject *parent = nullptr);
    virtual ~TaskScheduler();

    bool taskRunningCheck(QString objectname);

    //from TaskSchedulerConnector
    void addTask(const SchedulerCache &sc);
    void removeTask(QString objectname);

signals:
    //from EntryScheduler to Binder--------------------------------------------
    void timerStarted(QString objectname, QDateTime);
    void timerFinished(QString objectname, int);
    void encounteredScheduledTime(QString objectname);

    //from EntryTask to Binder-------------------------------------------------
    void processInitCount(QString objectname, int start, int end);
    void processCurrent(QString objectname, int currentnum);

    void processError(QString objectname, int currentnum);
    void processErrorText(QString objectname, QString str);
    void processMessage(QString objectname, QString, int type);

    void processStarted(QString objectname, int runfrom);
    void processPaused(QString objectname);
    void processStopped(QString objectname);
    void processEnd(QString objectname, int);

public slots:

    //from ui class
    void processPause(QString objectname);
    void processStop(QString objectname);
    void sendInput(QString objectname, QString text);

    //from EntryScheduler------------------------------------------------------
    void receiveTimerStarted(QDateTime datetime){emit timerStarted(sender()->objectName(), datetime);}
    void receiveTimerFinished(int value);
    void receiveEncounter();
    //from EntryTask-----------------------------------------------------------
    void receiveInitCount(int start, int end) { emit processInitCount(sender()->objectName(), start, end); }
    void receiveCurrent(int currentnum){emit processCurrent(sender()->objectName(), currentnum); }

    void receiveError(int currentnum){emit processError(sender()->objectName(), currentnum); }
    void receiveErrorText(QString str){emit processErrorText(sender()->objectName(), str); }
    void receiveMessage(QString message, int type){emit processMessage(sender()->objectName(), message, type); }

    void receiveStarted(int runfrom){emit processStarted(sender()->objectName(), runfrom); }
    void receivePaused(){emit processPaused(sender()->objectName()); }
    void receiveStopped(){emit processStopped(sender()->objectName()); }
    void receiveEnd(int type){emit processEnd(sender()->objectName(), type); }

private:
    bool processAliveCheck(QString objectname);

    QHash<QString, EntryTask *> *task;
    QHash<QString, EntryScheduler *> *scheduler;
    QMutex *basemutex;
};

#endif // TASKSCHEDULER_H
