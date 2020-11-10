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

#include "systemtray.h"

SystemTray::SystemTray(QWidget *parent) : QWidget(parent)
{
    //init trayicon and menu
    trayIcon = new QSystemTrayIcon(this);

    //init status widget
    strw = new StatusWidget();
    connect(strw, &StatusWidget::launchSettings, this, &SystemTray::launchSettingsAction);

    //load connect
    psw = strw->findChild<ProcessShowTable *>("enabledTableWidget");
    connect(psw, &ProcessShowTable::infoNofile, this, &SystemTray::showProcessFileEmpty);

    //set trayicon
    trayIcon->setIcon(QIcon(":/app_icons/app_abs_24x24_white.ico"));

    //connect action
    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::trayActivated);

    trayIconMenu = new QMenu(this);

    themeChangeAction();

}

SystemTray::~SystemTray()
{
    delete trayIconMenu;
    delete strw;
    delete trayIcon;
}

void SystemTray::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    strw->setTaskSchedulerConnector(task);

    connect(taskc, &TaskSchedulerConnector::timerStarted, this, &SystemTray::showTimerStart);
    connect(taskc, &TaskSchedulerConnector::timerFinished, this, &SystemTray::showTimerStopped);
    connect(taskc, &TaskSchedulerConnector::processStarted, this, &SystemTray::showProcessStart);
    connect(taskc, &TaskSchedulerConnector::processStopped, this, &SystemTray::showProcessStopped);
    connect(taskc, &TaskSchedulerConnector::processPaused, this, &SystemTray::showProcessPause);
    connect(taskc, &TaskSchedulerConnector::processEnd, this, &SystemTray::showProcessEnded);

    // Receive other ui or scheduler action change.
    connect(taskc, &TaskSchedulerConnector::updateState, this, &SystemTray::stateChanged);

    initTrayIcon();
}

void SystemTray::show(){ trayIcon->show();}
void SystemTray::hide(){ trayIcon->hide();}

void SystemTray::launchSettingsAction(){emit launchSetting();}
void SystemTray::trayCloseAction(){emit launchclose();}

void SystemTray::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        strw->showWidget();
        break;
    case QSystemTrayIcon::DoubleClick:
        launchSettingsAction();
        break;
    default:
        break;
    }
}

void SystemTray::showNotCloseMessage()
{
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
    settings.beginGroup(sc.GROUP_ABS);
    if(settings.value(sc.ABS_MINIMIZE, true).toBool()){
        trayIcon->showMessage(tr("AutoBatchScheduler is running")
                              , tr("To completely exit the program, right-click the icon in the task tray and choose Quit.")\
                              , QSystemTrayIcon::Information\
                              , settings.value(sc.ABS_MINIMIZE_MS, 2500).toInt());
    }
    settings.endGroup();
}

void SystemTray::showTimerStart(QString objectname, QDateTime time)
{
    //search valid data
    int itemid = getIndex(objectname);

    // load setting datas
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
    settings.beginGroup(sc.GROUP_ABS);
    bool started = settings.value(sc.ABS_TIMERSTART, true).toBool();
    int timerms = settings.value(sc.ABS_TIMERSTART_MS, 2500).toInt();
    settings.endGroup();

    qDebug() << "[SystemTray::showTimerStart] Timer started.";

    if(itemid >= 0 && started){
        SchedulerCache cache = taskc->read(itemid);
        QString textdata = QString("%1\n%2 %3")
                               .arg(cache.settingName)
                               .arg(time.toString(tr("yyyy/MM/dd HH:mm:ss")))
                               .arg(encodeDayOfWeek(time.date().dayOfWeek()));

        //show message
        trayIcon->showMessage(tr("Timer started"), textdata \
                              , QSystemTrayIcon::Information \
                              , timerms);

    }
}

void SystemTray::showTimerStopped(QString objectname, int type)
{

    qDebug() << "[SystemTray::showTimerStopped] Timer stopped.";

    switch (type) {
    case SchedulerWait::FINISHED:
    {
        //show message
        QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
        settings.beginGroup(sc.GROUP_ABS);
        if(settings.value(sc.ABS_TIMERSTOP, true).toBool()){
            trayIcon->showMessage(tr("Timer has ended"),\
                                  getNameByActions(objectname),\
                                  QSystemTrayIcon::Information,\
                                  settings.value(sc.ABS_TIMERSTOP_MS, 2500).toInt());
        }
        settings.endGroup();
        break;
    }
    case SchedulerWait::EXPIRED:
    {
        //show message
        trayIcon->showMessage(tr("The set time has already passed"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Warning,\
                              3500);

        int itemid = getIndex(objectname);
        if(itemid >= 0) taskc->disableSchedule(itemid);
        break;
    }
    default:
        break;
    }
}

void SystemTray::showProcessStart(QString objectname, int runfrom)
{
    if(runfrom == Executor::SCHEDULER){
        //show message
        QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
        settings.beginGroup(sc.GROUP_ABS);
        if(settings.value(sc.ABS_TASKSTART, true).toBool()){
            trayIcon->showMessage(tr("Task started"),\
                                  getNameByActions(objectname),\
                                  QSystemTrayIcon::Information,\
                                  settings.value(sc.ABS_TASKSTART_MS, 2500).toInt());
        }
        settings.endGroup();
    }
}

void SystemTray::showProcessPause(QString objectname)
{
    //show message
    trayIcon->showMessage(tr("The specified task is paused"),\
                          getNameByActions(objectname),\
                          QSystemTrayIcon::Information,\
                          3500);
}

void SystemTray::showProcessStopped(QString objectname)
{
    //show message
    trayIcon->showMessage(tr("Task stopped"),\
                          getNameByActions(objectname),\
                          QSystemTrayIcon::Information,\
                          3500);
}

void SystemTray::showProcessEnded(QString objectname, int type)
{
    if(type == Executor::SCHEDULER){
        //show message
        QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
        settings.beginGroup(sc.GROUP_ABS);
        if(settings.value(sc.ABS_TASKFINISHED, true).toBool()){
            trayIcon->showMessage(tr("Task ended normally"),\
                                  getNameByActions(objectname),\
                                  QSystemTrayIcon::Information,\
                                  settings.value(sc.ABS_TASKFINISHED_MS, 2500).toInt());
        }
        settings.endGroup();

    }
}

void SystemTray::showTaskDisabled(QString objectname)
{
    //show message
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
    settings.beginGroup(sc.GROUP_ABS);
    if(settings.value(sc.ABS_TASKUNSELECTED, true).toBool()){
        trayIcon->showMessage(tr("Task canceled"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Information,\
                              settings.value(sc.ABS_TASKUNSELECTED_MS, 2500).toInt());
    }
    settings.endGroup();
}

void SystemTray::showProcessFileEmpty(QString profilename)
{
    trayIcon->showMessage(tr("The profile to be executed could not be loaded."),\
                          profilename\
                          +tr("\r\nIn order to execute this schedule, please reset the execution profile"),\
                          QSystemTrayIcon::Warning,\
                          3500);
}

void SystemTray::stateChanged(int index, QString extra, TaskSchedulerConnector::TABLE action)
{
    qDebug() << "[SystemTray::stateChanged] index : " << index << " action : " << action;
    Q_UNUSED(index)

    switch (action) {
    case TaskSchedulerConnector::TABLE::ADD:
    case TaskSchedulerConnector::TABLE::INSERT:   break;
    case TaskSchedulerConnector::TABLE::DELETE:   break;
    case TaskSchedulerConnector::TABLE::EDIT:     break;
    case TaskSchedulerConnector::TABLE::ENABLE:   break;
    case TaskSchedulerConnector::TABLE::DISABLE:  showTaskDisabled(extra); break;
    case TaskSchedulerConnector::TABLE::DUPLICATE: break;
    case TaskSchedulerConnector::TABLE::DRAGDROP: break;
    case TaskSchedulerConnector::TABLE::UP: break;
    case TaskSchedulerConnector::TABLE::DOWN: break;
    case TaskSchedulerConnector::TABLE::SWAP: break;
    case TaskSchedulerConnector::TABLE::MOVE: break;
    }
}

void SystemTray::initTrayIcon()
{
    settingsAction = trayIconMenu->addAction(tr("Open Setting"));

    trayIconMenu->addSeparator();
    quitAction = trayIconMenu->addAction(tr("Quit"));

    connect(settingsAction, &QAction::triggered, this, &SystemTray::launchSettingsAction);
    connect(quitAction, &QAction::triggered, this, &SystemTray::trayCloseAction);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    trayIcon->setContextMenu(trayIconMenu);
}

int SystemTray::getIndex(QString objectname)
{
    QList<SchedulerCache> list = taskc->readAll();
    int count = list.count();

    for(int i = 0; i < count; i++){
        if(objectname == list.at(i).objectName()){
            return i;
        }
    }

    return -1;
}

QString SystemTray::getNameByActions(QString objectname)
{
    QList<QAction *> actlist = trayIconMenu->actions();
    QString result = "";
    foreach (QAction *act, actlist) {
        if(objectname == act->objectName()){
            result = act->text();
            break;
        }
    }

    return result;
}

QString SystemTray::encodeDayOfWeek(int dayofweek)
{
    switch (dayofweek) {
    case 1:  return tr("Monday");
    case 2:  return tr("Tuesday");
    case 3:  return tr("Wednesday");
    case 4:  return tr("Thursday");
    case 5:  return tr("Friday");
    case 6:  return tr("Saturday");
    case 7:  return tr("Sunday");
    default: break;
    }
    return "";
}

//QSS_THEME
void SystemTray::themeChangeAction()
{
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );

    //theme settings
    settings.beginGroup(sc.GROUP_ABS);
    QString stylecolor = settings.value(sc.ABS_THEME, "Default").toString();
    settings.endGroup();

    if(stylecolor != "Default"){
#ifdef QT_DEBUG
#ifdef Q_OS_WIN
        QFile file(QString("../../res/themes/%1.qss").arg(stylecolor));
#else
        QFile file(QString("../AutoBatchRunner-master/res/themes/%1.qss").arg(stylecolor));
#endif
#else
        QFile file(QString(":/themes/%1.qss").arg(stylecolor));
#endif
        if(file.open( QFile::ReadOnly | QFile::Text )){
            QString data(QLatin1String(file.readAll()));
            this->setStyleSheet(data);
            strw->setStyleSheet(data);
        }
    }
}
