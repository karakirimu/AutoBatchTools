/*
 * Copyright 2016-2021 karakirimu
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

#ifndef MULTITASKP_H
#define MULTITASKP_H

#include <QObject>
#include <QRandomGenerator>
#include <runtasksignalbinder.h>

// this class is almost copy of autobatchrunner/multitask.cpp
class MultiTaskP : public QObject
{
    Q_OBJECT
public:
    explicit MultiTaskP(QObject *parent = nullptr);
    ~MultiTaskP();

    bool taskRunningCheck(QString objectname);

    void addTask(QString objectname, QString processfile);
    void removeTask(QString objectname);

    void setInputFileList(QString objectname, QStringList *list);
    void setRange(QString objectname, QString str);

    QString generateRandom(int length);
signals:
    //from RunTaskSignalBinder to ui-------------------------------------------------
//  For multitask ui manage function
    void processInitCount(int start, int end);
    void processCurrent(int currentnum);

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

    //from RunTaskSignalBinder-----------------------------------------------------------
    // For multitask ui manage.
    void receiveInitCount(int start, int end) { emit processInitCount(start, end); }
    void receiveCurrent(int currentnum){emit processCurrent(currentnum); }

    void receiveError(int currentnum){emit processError(sender()->objectName(), currentnum); }
    void receiveErrorText(QString str){emit processErrorText(sender()->objectName(), str); }
    void receiveMessage(QString message, int type){emit processMessage(sender()->objectName(), message, type); }

    void receiveStarted(int runfrom){emit processStarted(sender()->objectName(), runfrom); /*emit processStarted_action(true);*/}
    void receivePaused(){emit processPaused(sender()->objectName()); /*emit processPaused_action(true);*/}
    void receiveStopped(){emit processStopped(sender()->objectName()); /*emit processStopped_action(true);*/}
    void receiveEnd(int type){emit processEnd(sender()->objectName(), type); /*emit processEnd_action(true);*/}

private:
    bool processAliveCheck(QString objectname);

    QHash<QString, RunTaskSignalBinder *> *task;
    QMutex *basemutex;
};

#endif // MULTITASKP_H
