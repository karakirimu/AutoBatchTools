/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QSystemTrayIcon>
#include <settingcache/startupxmlbuilder.h>
#include <statuswidget.h>
#include <taskschedulerconnector.h>
#include <QSettings>
#include <settingconstant.h>
#include "processshowtable.h"

class SystemTray : public QWidget
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = nullptr);
    ~SystemTray();

    void showNotCloseMessage();
    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

    void show();
    void hide();

signals:
    void launchSetting();
    void launchclose();

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);

    void showTimerStart(QString objectname, QDateTime time);
    void showTimerStopped(QString objectname, int type);
    void showProcessStart(QString objectname, int runfrom);
    void showProcessPause(QString objectname);
    void showProcessStopped(QString objectname);
    void showProcessEnded(QString objectname, int type);
    void showTaskDisabled(QString objectname);
    void showProcessFileEmpty(QString profilename);

    void stateChanged(int index, QString extra, TaskSchedulerConnector::TABLE action);

    void launchSettingsAction();
    void trayCloseAction();

private:
    void initTrayIcon();

    int getIndex(QString objectname);
    QString getNameByActions(QString objectname);
    QString encodeDayOfWeek(int dayofweek);
    void themeChangeAction();

    QMenu *trayIconMenu;
    QAction *launchAction;
    QAction *settingsAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    ProcessShowTable *psw;
    StatusWidget *strw;

    TaskSchedulerConnector *taskc;
    SettingConstant sc;

};

#endif // SYSTEMTRAY_H
