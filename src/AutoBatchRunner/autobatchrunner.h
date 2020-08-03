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

#ifndef AUTOBATCHRUNNER_H
#define AUTOBATCHRUNNER_H

#include "settingconstant.h"

#include <QMainWindow>
#include <QSettings>
#include <QProgressBar>
#include <QLabel>
#include <optiondialog.h>
#include <multitask.h>
#include <aboutabr.h>

namespace Ui {
class AutoBatchRunner;
}

class AutoBatchRunner : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoBatchRunner(QWidget *parent = nullptr);
    ~AutoBatchRunner();

private slots:

    //received from task
    void taskStarted(QString objectname, int runfrom);
    void taskPaused(QString objectname);
    void taskStopped(QString objectname);
    void taskEnd(QString objectname, int runfrom);

    //send to task
    void on_actionOpen_triggered();
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void on_actionSettings_triggered();

    void on_profileEditToolButton_clicked();

private:
    void setRunButtonState(bool run, bool pause, bool stop);
    void themeChangeAction();
    void initStatusBar();
    void about();

    Ui::AutoBatchRunner *ui;
    OptionDialog *opdialog;
    MultiTask *mlTask;

    //this variable is temporary.
    //currently, this program can run only one task.
    QString key = "";

    SettingConstant sc;
};

#endif // AUTOBATCHRUNNER_H
