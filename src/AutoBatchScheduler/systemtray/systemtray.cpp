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
    trayIcon->setIcon(QIcon(":/icons/app_systray.png"));

    //connect action
    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::trayActivated);

    trayIconMenu = new QMenu(this);

    //set new xml builder
    builder = new StartupXmlBuilder();

}

SystemTray::~SystemTray()
{
    delete builder;
    delete trayIconMenu;
    delete strw;
    delete trayIcon;
}

void SystemTray::showNotCloseMessage()
{
    trayIcon->showMessage(tr("AutoBatchSchedulerは起動しています")
                          , tr("プログラムを完全に終了する場合は、タスクトレイにあるアイコンを右クリックし、終了を選択してください")\
                          , QSystemTrayIcon::Information\
                          , 3000);
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

//void SystemTray::onCheckStateChanged(bool checked)
//{
//     QString objname = this->sender()->objectName();
//     int itemid = getStartupXmlIndex(objname);

//     if(itemid < 0) return;

//     if(checked){
//         QList<QStringList> list;
//         if(builder->readItem(itemid, &list)){
//             QFileInfo info(list.at(StartupXmlBuilder::PROF).at(1));
//             if(info.exists()){
//                 //change xml data (warning : determined order)
//                 changeXmlValidState(itemid);
//                 taskc->enableTask(objname, info.canonicalFilePath());

//             }else{
//                 //show message
//                 trayIcon->showMessage(tr("実行用プロファイルが存在しません"),\
//                                       getNameByActions(objname)\
//                                       +tr("\r\nこのスケジュールを実行するためには、実行用プロファイルを再設定してください"),\
//                                       QSystemTrayIcon::Warning,\
//                                       3500);
//             }
//         }
//     }else{
//         taskc->disableTask(objname);
//         //change xml data
//         changeXmlValidState(itemid);
//     }
//}

//void SystemTray::updateCheckStateChanged(QString objname)
//{
//    QList<QAction *> actlist = trayIconMenu->actions();
//    QAction *act;
//    int count = actlist.count();

//    for(int i = 0; i < count; i++) {
//        act = actlist.at(i);
//        if(objname == act->objectName()){
//            act->setChecked(!act->isChecked());
//            break;
//        }
//    }
//}

void SystemTray::launchSettingsAction(){emit launchSetting();}
void SystemTray::trayCloseAction(){emit launchclose();}

//void SystemTray::addlistAction(int xmlitemid)
//{
////    if(xmlitemid == 0){
////        trayIconMenu->insertAction(trayIconMenu->actions().at(0), generateAction(xmlitemid));
////    }else{
//        int count = trayIconMenu->actions().count() - 2;
//        trayIconMenu->insertAction(trayIconMenu->actions().at(count), generateAction(xmlitemid));
////    }
//}

//void SystemTray::deletelistAction(QString objectname)
//{
//    //xml data deleted
//    QList<QAction *> actlist = trayIconMenu->actions();
//    QAction *act;
//    int count = actlist.count();

//    for(int i = 0; i < count; i++) {
//        act = actlist.at(i);
//        if(objectname == act->objectName()){
//            break;
//        }
//    }

//    if(act->isChecked()){
//        //when task is started
//        taskc->disableTask(objectname);
//    }

//    trayIconMenu->removeAction(act);
//}

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

void SystemTray::showTimerStart(QString objectname, QDateTime time)
{
    //search valid data
    int itemid = getStartupXmlIndex(objectname);

    // load setting datas
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    bool hidestarted = settings.value("HIDETIMERSTART", false).toBool();
    int timerms = settings.value("TIMERSTARTMS", 2500).toInt();
    settings.endGroup();

    qDebug() << "(timer started)";

    if(itemid < 0 || hidestarted) return;

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

void SystemTray::showTimerStopped(QString objectname, int type)
{   

    qDebug() << "(timer stopped)";

    switch (type) {
    case SchedulerWait::FINISHED:
    {
        //show message
        trayIcon->showMessage(tr("タイマーは終了しました"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Information,\
                              3500);
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
    switch (runfrom) {
    case Executor::DEFAULT:
        break;
    case Executor::SCHEDULER:
        //show message
        trayIcon->showMessage(tr("タスクを開始しました"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Information,\
                              3500);
        break;
    default:
        break;
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
    switch (type) {
    case Executor::MAINPROCESS:
        //show message
        trayIcon->showMessage(tr("タスクは正常終了しました"),\
                              getNameByActions(objectname),\
                              QSystemTrayIcon::Information,\
                              3500);
        break;
    case Executor::OTHERPROCESS:
        break;
    default:
        break;
    }
}

void SystemTray::showTaskDisabled(QString objectname)
{
    //show message
    trayIcon->showMessage(tr("タスクを解除しました"),\
                          getNameByActions(objectname),\
                          QSystemTrayIcon::Information,\
                          3500);
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
    //disconnect action
//    disconnect(launchAction, &QAction::triggered, this, &SystemTray::launchMainAction);
//    disconnect(settingsAction, &QAction::triggered, this, &SystemTray::launchSettingsAction);
//    disconnect(quitAction, &QAction::triggered, this, &QObject::deleteLater);

//    launchAction = trayIconMenu->addAction(tr("メインウィンドウを開く…"));
    settingsAction = trayIconMenu->addAction(tr("設定を開く…"));

    //set list from profilexml
//    initDynamicActionList();

    //add variant item
    trayIconMenu->addSeparator();
    quitAction = trayIconMenu->addAction(tr("終了"));

//    connect(launchAction, &QAction::triggered, this, &SystemTray::launchMainAction);
    connect(settingsAction, &QAction::triggered, this, &SystemTray::launchSettingsAction);
    connect(quitAction, &QAction::triggered, this, &SystemTray::trayCloseAction);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    trayIcon->setContextMenu(trayIconMenu);
}

//void SystemTray::initDynamicActionList()
//{
//    int count = builder->count();
//    if(count > 0) trayIconMenu->addSeparator();

//    for(int i = 0; i < count; i++){
//        initDynamicAction(i);
//    }
//}

////DEPENDS_XML
//void SystemTray::initDynamicAction(int itemid)
//{
//    trayIconMenu->addAction(generateAction(itemid));
//}



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

//QAction *SystemTray::generateAction(int itemid)
//{
//    QList<QStringList> *list = new QList<QStringList>();

//    QAction *dyact;
//    if(builder->readItem(itemid, list)){
//        dyact = new QAction(list->at(StartupXmlBuilder::NAME).at(1), this);
//        bool isvalid = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? true : false;

//        //set checked action
//        dyact->setCheckable(true);
//        dyact->setChecked(isvalid);

//        //set object name
//        dyact->setObjectName(list->at(StartupXmlBuilder::UNIQUE).at(1));

//        //start scheduler if checkbox is valid
//        if(isvalid){
//            QFileInfo info(list->at(StartupXmlBuilder::PROF).at(1));
//            if(info.exists()){
//                taskc->enableTask(list->at(StartupXmlBuilder::UNIQUE).at(1), info.canonicalFilePath());

//            }else{
//                //change xml data (warning : determined order)
//                changeXmlValidState(itemid);
//                //show message
//                trayIcon->showMessage(tr("実行用プロファイルが存在しません"),\
//                                      getNameByActions(list->at(StartupXmlBuilder::UNIQUE).at(1))\
//                                      +tr("\r\nこのスケジュールを実行するためには、実行用プロファイルを再設定してください"),\
//                                      QSystemTrayIcon::Warning,\
//                                      3500);
//            }
//        }

//        connect(dyact, SIGNAL(triggered(bool)), this, SLOT(onCheckStateChanged(bool)));
//    }

//    delete list;
//    return dyact;
//}
