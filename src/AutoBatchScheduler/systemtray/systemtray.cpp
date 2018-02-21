#include "systemtray.h"

#include <QSettings>

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
    trayIcon->setIcon(QIcon(":/app_icons/app_abs.ico"));

    //connect action
    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::trayActivated);

    trayIconMenu = new QMenu(this);

    //set new xml builder
    builder = new StartupXmlBuilder();

//    settings.setDefaultFormat(QSettings::IniFormat);
//    settings.setUserIniPath("./settings.ini");
}

SystemTray::~SystemTray()
{
    delete builder;
    delete trayIconMenu;
    delete strw;
    delete trayIcon;
}

void SystemTray::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    strw->setTaskSchedulerConnector(task);

    //connect
    connect(taskc, &TaskSchedulerConnector::timerStarted, this, &SystemTray::showTimerStart);
    connect(taskc, &TaskSchedulerConnector::timerFinished, this, &SystemTray::showTimerStopped);
    connect(taskc, &TaskSchedulerConnector::processStarted, this, &SystemTray::showProcessStart);
    connect(taskc, &TaskSchedulerConnector::processStopped, this, &SystemTray::showProcessStopped);
    connect(taskc, &TaskSchedulerConnector::processPaused, this, &SystemTray::showProcessPause);
    connect(taskc, &TaskSchedulerConnector::processEnd, this, &SystemTray::showProcessEnded);

    //not timer this is whole task
    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &SystemTray::showTaskDisabled);

//    connect(taskc, &TaskSchedulerConnector::addListAction, this, &SystemTray::addlistAction);
//    connect(taskc, &TaskSchedulerConnector::deleteListAction, this, &SystemTray::deletelistAction);

    //Menu Checkable
//    connect(taskc, &TaskSchedulerConnector::xmlStateChanged, this, &SystemTray::updateCheckStateChanged);

    //set trayicon actions (task function is need for this.)
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
        strw->show();
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
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    if(settings.value("MINIMIZESHOW", true).toBool()){
        trayIcon->showMessage(tr("AutoBatchSchedulerは起動しています")
                              , tr("プログラムを完全に終了する場合は、タスクトレイにあるアイコンを右クリックし、終了を選択してください")\
                              , QSystemTrayIcon::Information\
                              , settings.value("MINIMIZESHOWMS", 2500).toInt());
    }
    settings.endGroup();
}

void SystemTray::showTimerStart(QString objectname, QDateTime time)
{
    //search valid data
    int itemid = getStartupXmlIndex(objectname);

    // load setting datas
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    bool started = settings.value("TIMERSTART", true).toBool();
    int timerms = settings.value("TIMERSTARTMS", 2500).toInt();
    settings.endGroup();

    qDebug() << "(timer started)";

    if(itemid >= 0 && started){
        //get newtime from xml
        QList<QStringList> *list = new QList<QStringList>();

        QString textdata;
        //maybe slow
        if(builder->readItem(itemid, list)){
            textdata.append(list->at(StartupXmlBuilder::NAME).at(1));
            textdata.append("\r\n");
            textdata.append(time.toString("yyyy/MM/dd HH:mm:ss"));
            textdata.append(" ");
            textdata.append(encodeDayOfWeek(time.date().dayOfWeek()));

        }

        //show message
        trayIcon->showMessage(tr("タイマーを開始しました"), textdata\
                              , QSystemTrayIcon::Information\
                              , timerms);

        delete list;
    }
}

void SystemTray::showTimerStopped(QString objectname, int type)
{   

    qDebug() << "(timer stopped)";

    switch (type) {
    case SchedulerWait::FINISHED:
    {
        //show message
        QSettings settings( "./settings.ini", QSettings::IniFormat );
        settings.beginGroup("STARTUP");
        if(settings.value("TIMERSTOP", true).toBool()){
            trayIcon->showMessage(tr("タイマーは終了しました"),\
                                  getNameByActions(objectname),\
                                  QSystemTrayIcon::Information,\
                                  settings.value("TIMERSTOPMS", 2500).toInt());
        }
        settings.endGroup();
        break;
    }
    case SchedulerWait::EXPIRED:
    {
        //show message
        trayIcon->showMessage(tr("設定した時刻は既に過ぎています"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Warning,\
                              3500);
        //change action checkstate
        int itemid = getStartupXmlIndex(objectname);
        if(itemid >= 0) changeXmlValidState(itemid);
//        updateCheckStateChanged(objectname);
        taskc->disableTask(objectname);
        break;
    }
    default:
        break;
    }
}

void SystemTray::showProcessStart(QString objectname, int runfrom)
{
//    switch (runfrom) {
//    case Executor::DEFAULT:
//        break;
//    case Executor::SCHEDULER:
//        //show message
//        QSettings settings( "./settings.ini", QSettings::IniFormat );
//        settings.beginGroup("STARTUP");
//        if(settings.value("TASKSTART", true).toBool()){
//            trayIcon->showMessage(tr("タスクを開始しました"),\
//                                  getNameByActions(objectname),\
//                                  QSystemTrayIcon::Information,\
//                                  settings.value("TASKSTOPMS", 2500).toInt());
//        }
//        settings.endGroup();

//        break;
//    default:
//        break;
//    }

    if(runfrom == Executor::SCHEDULER){
        //show message
        QSettings settings( "./settings.ini", QSettings::IniFormat );
        settings.beginGroup("STARTUP");
        if(settings.value("TASKSTART", true).toBool()){
            trayIcon->showMessage(tr("タスクを開始しました"),\
                                  getNameByActions(objectname),\
                                  QSystemTrayIcon::Information,\
                                  settings.value("TASKSTOPMS", 2500).toInt());
        }
        settings.endGroup();
    }
}

void SystemTray::showProcessPause(QString objectname)
{
    //show message
    trayIcon->showMessage(tr("指定したタスクは一時停止中です"),\
                          getNameByActions(objectname),\
                          QSystemTrayIcon::Information,\
                          3500);
}

void SystemTray::showProcessStopped(QString objectname)
{
    //show message
    trayIcon->showMessage(tr("タスクは停止しました"),\
                          getNameByActions(objectname),\
                          QSystemTrayIcon::Information,\
                          3500);
}

void SystemTray::showProcessEnded(QString objectname, int type)
{
//    switch (type) {
//    case Executor::MAINPROCESS:
//        //show message
//        QSettings settings( "./settings.ini", QSettings::IniFormat );
//        settings.beginGroup("STARTUP");
//        if(settings.value("TASKEND", true).toBool()){
//            trayIcon->showMessage(tr("タスクは正常終了しました"),\
//                                  getNameByActions(objectname),\
//                                  QSystemTrayIcon::Information,\
//                                  settings.value("TASKENDMS", 2500).toInt());
//        }
//        settings.endGroup();
//        break;
//// obsolete
////    case Executor::OTHERPROCESS:
////        break;
//    default:
//        break;
//    }

    if(type == Executor::MAINPROCESS){
        //show message
        QSettings settings( "./settings.ini", QSettings::IniFormat );
        settings.beginGroup("STARTUP");
        if(settings.value("TASKEND", true).toBool()){
            trayIcon->showMessage(tr("タスクは正常終了しました"),\
                                  getNameByActions(objectname),\
                                  QSystemTrayIcon::Information,\
                                  settings.value("TASKENDMS", 2500).toInt());
        }
        settings.endGroup();

    }
}

void SystemTray::showTaskDisabled(QString objectname)
{
    //show message
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    if(settings.value("TASKUNSELECT", true).toBool()){
        trayIcon->showMessage(tr("タスクを解除しました"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Information,\
                              settings.value("TASKUNSELECTMS", 2500).toInt());
    }
    settings.endGroup();
}

void SystemTray::showProcessFileEmpty(QString profilename)
{
    trayIcon->showMessage(tr("実行用プロファイルが存在しません"),\
                          profilename\
                          +tr("\r\nこのスケジュールを実行するためには、実行用プロファイルを再設定してください"),\
                          QSystemTrayIcon::Warning,\
                          3500);
}

void SystemTray::initTrayIcon()
{
    settingsAction = trayIconMenu->addAction(tr("設定を開く…"));

    trayIconMenu->addSeparator();
    quitAction = trayIconMenu->addAction(tr("終了"));

//    connect(launchAction, &QAction::triggered, this, &SystemTray::launchMainAction);
    connect(settingsAction, &QAction::triggered, this, &SystemTray::launchSettingsAction);
    connect(quitAction, &QAction::triggered, this, &SystemTray::trayCloseAction);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    trayIcon->setContextMenu(trayIconMenu);
}

void SystemTray::changeXmlValidState(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    if(builder->readItem(itemid, list)){

        QString identifier = (list->at(2).at(1) == "yes")? "no" : "yes";

        //change validation
        QStringList tmp;
        tmp << "valid" << identifier;
        list->removeAt(2);
        list->insert(2, tmp);

        builder->editItem(itemid, list);

//        emit taskc->xmlStateChanged(list->at(StartupXmlBuilder::UNIQUE).at(1));
    }

    delete list;
}

int SystemTray::getStartupXmlIndex(QString objectname)
{
    QList<QStringList> *list = new QList<QStringList>();

    //search valid data
    int itemid = -1;
    int count = builder->count();

    for(int i = 0; i < count; i++){
        list->clear();
        if(builder->readItem(i, list)
                && objectname == list->at(StartupXmlBuilder::UNIQUE).at(1)){
            itemid = i;
            break;
        }
    }

    delete list;
    return itemid;
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
    case 1:
        return tr("月曜日");
    case 2:
        return tr("火曜日");
    case 3:
        return tr("水曜日");
    case 4:
        return tr("木曜日");
    case 5:
        return tr("金曜日");
    case 6:
        return tr("土曜日");
    case 7:
        return tr("日曜日");
    default:
        break;
    }
    return "";
}
