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

#ifndef PROCESSSHOWTABLE_H
#define PROCESSSHOWTABLE_H

#include <QObject>
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <taskschedulerconnector.h>
#include "cellinfowidget.h"
#include "consolewidget.h"

#define RANGE 30

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
    //create ui
//    void addItem(QString objname);
    void tableChanged(QString message, TaskSchedulerConnector::TABLE func);


    //from startuptable
    void removeItem(int itemid);
    void insertItem(int itemid);
    void replaceItem(int itemid);

    //from tasksignalconnector
    void enableItem(QString objname);
    void disableItem(QString objname);

    //send command to parent
    void pauseClicked(QString objname){emit pause(objname);}
    void stopClicked(QString objname){emit stop(objname);}

    //receive command from parent
    void setProgressRange(QString objname, int start, int end);
    void updateProgressBar(QString objname, int num);
    void setUpdateProcess(QString objname, QString data, int type);

    void encounterdTime(QString objname);
    void timerStarted(QString objname, QDateTime date);
    void setProcessEnd(QString objname, int type);
    void setProcessStopped(QString objname);
    void setProcessError(QString objname, int currentnum);
    void setProcessErrorText(QString objectname, QString str);

    //launch in this class
    void launchConsole(QString objname);

private slots:
    void onCheckStateChanged(bool checked);

private:
//    bool eventFilter(QObject *obj, QEvent *event);
    void initCellWidgets();
    void initCellWidget(int itemid);
    void changeXmlValidState(int itemid);
    int getStartupXmlIndex(QString objectname);

    CellInfoWidget *getCellWidget(QString objname);

    StartupXmlBuilder *builder;
    TaskSchedulerConnector *taskc;
};

#endif // PROCESSSHOWTABLE_H
