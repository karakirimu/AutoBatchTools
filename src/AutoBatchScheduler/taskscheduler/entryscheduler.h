/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef ENTRYSCHEDULER_H
#define ENTRYSCHEDULER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <schedulerwait.h>
#include <settingcache/startupxmlbuilder.h>

class EntryScheduler : public QObject
{
    Q_OBJECT
public:
    explicit EntryScheduler(QObject *parent = nullptr);
    ~EntryScheduler();

    bool isStarted();
    void setRefreshTime(ulong milliseconds); //preset
    void setMutex(QMutex *mutex); //preset

    SchedulerWait *getSchedulerWait() const { return swait; }

public slots:
    void start();
    void stop();

private:
    SchedulerWait *swait;
    QThread *worker;

};

#endif // ENTRYSCHEDULER_H
