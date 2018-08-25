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
    trayIcon->setIcon(QIcon(":/app_icons/app_abs_24x24_white.ico"));

    //connect action
    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::trayActivated);

    trayIconMenu = new QMenu(this);

    //set new xml builder
    builder = new StartupXmlBuilder();

    themeChangeAction();

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
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("scheduler_startup");
    if(settings.value("MINIMIZESHOW", true).toBool()){
        trayIcon->showMessage(tr("AutoBatchScheduler is running")
                              , tr("To completely exit the program, right-click the icon in the task tray and choose Exit.")\
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
    settings.beginGroup("scheduler_startup");
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
        trayIcon->showMessage(tr("Timer started"), textdata\
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
        settings.beginGroup("scheduler_startup");
        if(settings.value("TIMERSTOP", true).toBool()){
            trayIcon->showMessage(tr("Timer has ended"),\
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
        trayIcon->showMessage(tr("The set time has already passed"),\
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
//        settings.beginGroup("scheduler_startup");
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
        settings.beginGroup("scheduler_startup");
        if(settings.value("TASKSTART", true).toBool()){
            trayIcon->showMessage(tr("Task started"),\
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
//    switch (type) {
//    case Executor::MAINPROCESS:
//        //show message
//        QSettings settings( "./settings.ini", QSettings::IniFormat );
//        settings.beginGroup("scheduler_startup");
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

    if(type == Executor::SCHEDULER){
        //show message
        QSettings settings( "./settings.ini", QSettings::IniFormat );
        settings.beginGroup("scheduler_startup");
        if(settings.value("TASKEND", true).toBool()){
            trayIcon->showMessage(tr("Task ended normally"),\
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
    settings.beginGroup("scheduler_startup");
    if(settings.value("TASKUNSELECT", true).toBool()){
        trayIcon->showMessage(tr("Task canceled"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Information,\
                              settings.value("TASKUNSELECTMS", 2500).toInt());
    }
    settings.endGroup();
}

void SystemTray::showProcessFileEmpty(QString profilename)
{
    trayIcon->showMessage(tr("Profile does not exist for execution"),\
                          profilename\
                          +tr("\r\nIn order to execute this schedule, please reset the execution profile"),\
                          QSystemTrayIcon::Warning,\
                          3500);
}

void SystemTray::initTrayIcon()
{
    settingsAction = trayIconMenu->addAction(tr("Open settings ..."));

    trayIconMenu->addSeparator();
    quitAction = trayIconMenu->addAction(tr("Quit"));

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

        QString identifier = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? "no" : "yes";

        //change validation
        list->removeAt(StartupXmlBuilder::VALID);
        list->insert(StartupXmlBuilder::VALID, QStringList() << "valid" << identifier);

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
        return tr("Monday");
    case 2:
        return tr("Tuesday");
    case 3:
        return tr("Wednesday");
    case 4:
        return tr("Thursday");
    case 5:
        return tr("Friday");
    case 6:
        return tr("Saturday");
    case 7:
        return tr("Sunday");
    default:
        break;
    }
    return "";
}

//QSS_THEME
void SystemTray::themeChangeAction()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("scheduler_startup");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
    settings.endGroup();

    if(stylecolor != "Default"){
#ifdef QT_DEBUG
        QFile file(QString("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/%1.qss").arg(stylecolor));
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
