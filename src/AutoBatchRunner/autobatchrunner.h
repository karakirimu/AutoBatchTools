/*****************************************************************************
 * autobatchrunner.h: interfaces handling
 *****************************************************************************
 * Copyright (C) 2016-2018
 * $Id$
 *
 * Author: k-vontade
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef AUTOBATCHRUNNER_H
#define AUTOBATCHRUNNER_H

#include <QMainWindow>
#include <QSettings>
//#include <systemtray.h>
#include <QProgressBar>
#include <QLabel>
#include <optiondialog.h>
#include <multitask.h>
//#include <mainprocessloader.h>
//#include <profileeditor.h>

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

    Ui::AutoBatchRunner *ui;
    OptionDialog *opdialog;
    MultiTask *mlTask;

    //this variable is temporary.
    //currently, this program can run only one task.
    QString key = "";
};

#endif // AUTOBATCHRUNNER_H
