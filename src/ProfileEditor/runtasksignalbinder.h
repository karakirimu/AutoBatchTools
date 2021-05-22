/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef RUNTASKSIGNALBINDER_H
#define RUNTASKSIGNALBINDER_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include "../executor/executor.h"
#include "settingconstant.h"

class RunTaskSignalBinder : public QObject
{
    Q_OBJECT
public:
    explicit RunTaskSignalBinder(QObject *parent = nullptr);
    ~RunTaskSignalBinder();

    bool getStarted();
    bool getPause();
    void setMutex(QMutex *sharedmutex);
    void setFile(QString filepath);

    inline Executor *getExecutor() { return executor; }

public slots:
    void sendInput(QString message);

    // from menu
    void start();
    void pause(bool pause);
    void stop();

    // from settest dock
    void updateRange(QString str);
    void updateFileList(QStringList *data);

private:
    QMutex *mutex;
    Executor *executor;
    QThread *worker;

};

#endif // RUNTASKSIGNALBINDER_H
