/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ENTRYTASK_H
#define ENTRYTASK_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QSettings>
#include "../executor/executor.h"

/**
 * @brief The EntryTask class
 *
 * This class is copy of autobatchscheduler/entrytask
 */
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
    void updateFileList(QStringList *need); /*int indicates status*/

private slots:
    //debug message
    void processCompleted();

private:
    QMutex *mutex;
    Executor *executor;
    QThread *worker;
};

#endif // ENTRYTASK_H
