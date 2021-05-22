/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROCESSSHOWTABLE_H
#define PROCESSSHOWTABLE_H

#include <QObject>
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <taskschedulerconnector.h>
#include "cellinfowidget.h"
#include "consolewidget.h"
#include "statuswidget.h"

class ProcessShowTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit ProcessShowTable(QWidget *parent = nullptr);
    ~ProcessShowTable();

    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

signals:
    //from cellwidget
    void pause(QString);
    void stop(QString);

    //to systemtray
    void infoNofile(QString);

public slots:
    void stateChanged(int index, QString message, TaskSchedulerConnector::TABLE func);

    void removeItem(int itemid);
    void insertItem(int itemid);
    void replaceItem(int itemid);
    void moveItem(int before, int after);
    void duplicateItem();

    //from tasksignalconnector
    void enableItem(int index);
    void disableItem(int index);

    //send command to parent
    void pauseClicked(QString objname){emit pause(objname);}
    void stopClicked(QString objname){emit stop(objname);}

    //receive command from parent
    void setProgressRange(QString objname, int start, int end);
    void updateProgressBar(QString objname, int num);
    void setUpdateProcess(QString objname, QString data, int type);

    void encounterdTime(QString objname);
    void timerStarted(QString objname, QDateTime date);
    void timerStopped(QString objectname, int type);
    void setProcessEnd(QString objname, int type);
    void setProcessStopped(QString objname);
    void setProcessError(QString objname, int currentnum);
    void setProcessErrorText(QString objectname, QString str);

    //launch in this class
    void launchConsole(QString objname);

private slots:
    void onCheckStateChanged(bool checked);
    void initCellWidgets();

private:
    void initCellWidget(int itemid);
    void initCellWidget(int itemid, const SchedulerCache &cache);
    int getIndex(QString objectname);

    CellInfoWidget *getCellWidget(QString objname);

    TaskSchedulerConnector *taskc;
};

#endif // PROCESSSHOWTABLE_H
