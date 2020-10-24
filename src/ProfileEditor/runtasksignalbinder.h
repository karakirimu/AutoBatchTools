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
