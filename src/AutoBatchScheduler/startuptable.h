/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STARTUPTABLE_H
#define STARTUPTABLE_H

#include <../basictable/basictable.h>
#include <settingcache/startupxmlbuilder.h>
#include <../variantconverter/variantconverter.h>
#include "startupdialog.h"
#include "taskschedulerconnector.h"
#include <../share/qss/qsspropertyconstant.h>

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

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event);
    int getIndex(QString objectname);

    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;
    QAction *m_enable;
    QAction *m_disable;

    TaskSchedulerConnector *taskc;
    void setTableItem(int row);
    void setTableItem(int row, const SchedulerCache &sc);

    void replaceItem(int row);
};

#endif // STARTUPTABLE_H
