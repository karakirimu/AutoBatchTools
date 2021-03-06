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

#ifndef COMMANDLINETEXTEDIT_H
#define COMMANDLINETEXTEDIT_H

#include <QEvent>
#include <QWidget>
#include <multitask.h>
#include <../consolebase/consolebase.h>

/**
 * @brief The ConsoleView class
 * This class may be same as SchedulerConsoleView
 */

class ConsoleView : public ConsoleBase
{
    Q_OBJECT
public:
    explicit ConsoleView(QObject *parent = nullptr);
    ~ConsoleView();

    //ui settings
    void setMultiTask(MultiTask *mltask);
    void setReadObjectName(QString objname);

private slots:
    void startedMessage(QString obj, int runtype);
    void pausedMessage(QString obj);
    void stoppedMessage(QString obj);
    void endMessage(QString obj, int runtype);
    void updateMessage(QString obj, QString data, int runtype);
    void errorMessage(QString obj, QString str);


private:
    QString objname;
};

#endif // COMMANDLINETEXTEDIT_H
