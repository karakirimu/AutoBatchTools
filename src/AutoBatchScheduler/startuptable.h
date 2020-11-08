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

#ifndef STARTUPTABLE_H
#define STARTUPTABLE_H

#include <../basictable/basictable.h>
#include <settingcache/startupxmlbuilder.h>
#include <../variantconverter/variantconverter.h>
#include "startupdialog.h"
#include "taskschedulerconnector.h"

#include <QSettings>

class StartupTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StartupTable(QWidget *parent = nullptr);
    ~StartupTable();
    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

private slots:
    void stateChanged(int index, QString message, TaskSchedulerConnector::TABLE func);

    void addAction();
    void editTableAction(int row, int col);
    void editAction();
    void deleteAction();
    void copyAction();
    void upAction();
    void downAction();
    void enableAction();
    void disableAction();
    void reloadAction();

//    void updateItemEnabled(QString objectname);
//    void updateItemTimerEnd(QString objectname, int status);

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event);
    int getStartupXmlIndex(QString objectname);

    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;
    QAction *m_enable;
    QAction *m_disable;

//    StartupXmlBuilder *builder;
    TaskSchedulerConnector *taskc;
    void setTableItem(int row);
    void setTableItem(int row, const SchedulerCache &sc);

    void replaceItem(int row);
//    QString getRandomString(int length);
};

#endif // STARTUPTABLE_H
