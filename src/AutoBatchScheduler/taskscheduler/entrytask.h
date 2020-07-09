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
#include "../executor/executor.h"

class EntryTask : public QObject
{
    Q_OBJECT
public:
    explicit EntryTask(QObject *parent = nullptr);
    ~EntryTask();

    void setFile(QString filepath);
    void setMutex(QMutex *sharedmutex);

    bool getStarted();
    bool getPause();

signals:
    //for progress
    void processInitCount(int start, int end);
    void processCurrent(int currentnum);

    //for progress and menu
    void processError(int currentnum);

    //for textedit
    void processErrorText(QString str);
    void processMessage(QString, int type);

    //for all ui
    void processStarted(int runfrom);
//    void processStarted_action(bool);

    void processPaused();
//    void processPaused_action(bool);

    void processStopped();
//    void processStopped_action(bool);

    void processEnd(int);
//    void processEnd_action(bool);

public slots:
    //for progress
    void receiveInitCount(int start, int end) { emit processInitCount(start, end); }
    void receiveCurrent(int currentnum){emit processCurrent(currentnum); }

    //for progress and menu
    void receiveError(int currentnum){emit processError(currentnum); }

    //for textedit
    void receiveErrorText(QString str){emit processErrorText(str); }
    void receiveMessage(QString message, int type){emit processMessage(message, type); }
    void sendInput(QString message);

    //for all ui
    void receiveStarted(int runfrom){emit processStarted(runfrom); /*emit processStarted_action(true);*/}
    void receivePaused(){emit processPaused(); /*emit processPaused_action(true);*/}
    void receiveStopped(){emit processStopped(); /*emit processStopped_action(true);*/}
    void receiveEnd(int type){emit processEnd(type); /*emit processEnd_action(true);*/}

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
//    QString loadfilepath = "";
};

#endif // ENTRYTASK_H
