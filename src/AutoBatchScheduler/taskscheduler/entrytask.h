/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef ENTRYTASK_H
#define ENTRYTASK_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include "../executor/executor.h"

class EntryTask : public QObject
{
    Q_OBJECT
public:
    explicit EntryTask(QObject *parent = nullptr);
    ~EntryTask();

    bool getStarted();
    bool getPause();
    void setFile(QString filepath);
    void setMutex(QMutex *sharedmutex);

    inline Executor *getExecutor() { return executor; }

public slots:
    void sendInput(QString message);

    //from menu
    void start();
    void pause(bool paused);
    void stop();

    //from settest dock
    void updateFileList(QHash<QString, int> *data); /*int indicates status*/

private slots:
    //debug message
    void processCompleted();
private:
    QMutex *mutex;
    Executor *executor;
    QThread *worker;
};

#endif // ENTRYTASK_H
