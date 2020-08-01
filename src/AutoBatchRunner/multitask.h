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

#ifndef MULTITASK_H
#define MULTITASK_H

#include <QObject>
#include <QTime>
#include <entrytask.h>
#include <QRandomGenerator>

class MultiTask : public QObject
{
    Q_OBJECT
public:
    explicit MultiTask(QObject *parent = nullptr);
    ~MultiTask();

    bool taskRunningCheck(QString objectname);

    void addTask(QString objectname, QString processfile);
    void removeTask(QString objectname);

    void setInputFileList(QString objectname, QStringList *list);

    QString generateRandom(int length);
signals:
    // from EntryTask to ui-------------------------------------------------
    // For multitask ui manage function
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

    // from EntryTask-----------------------------------------------------------
    // For multitask ui manage. Currently it does singletask.
    void receiveInitCount(int start, int end) { emit processInitCount(start, end); }
    void receiveCurrent(int currentnum){emit processCurrent(currentnum); }

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
    QMutex *basemutex;
//    QRandomGenerator rgen;
};

#endif // MULTITASK_H
