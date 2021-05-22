/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
