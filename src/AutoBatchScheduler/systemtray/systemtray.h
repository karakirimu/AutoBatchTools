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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QSystemTrayIcon>
#include <startupxmlbuilder.h>
#include <statuswidget.h>
#include <taskschedulerconnector.h>
#include <QSettings>
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

    void launchSettingsAction();
    void trayCloseAction();

private:
    void initTrayIcon();

    void changeXmlValidState(int itemid);
    int getStartupXmlIndex(QString objectname);
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

    StartupXmlBuilder *builder;
    TaskSchedulerConnector *taskc;

};

#endif // SYSTEMTRAY_H
