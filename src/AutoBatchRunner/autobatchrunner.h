/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
    ~AutoBatchRunner() override;

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
