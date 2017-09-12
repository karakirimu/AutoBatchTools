#include "systemtray.h"

#include <QSettings>

SystemTray::SystemTray(QWidget *parent) : QWidget(parent)
{
    //init trayicon and menu
    trayIcon = new QSystemTrayIcon(this);

    //set trayicon
    trayIcon->setIcon(QIcon(":/icons/app_systray.png"));

    //connect action
    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::trayActivated);

    trayIconMenu = new QMenu(this);

    //set new xml builder
    builder = new StartupXmlBuilder();

    //set timemanager (schedule timer)
    timer = new TimeManager();

    //connect timer
    connect(timer, SIGNAL(timerStarted(QString)), this, SLOT(showTimerStart(QString)));
    connect(timer, SIGNAL(timerStopped(QString)), this, SLOT(showTimerStopped(QString)));
    connect(timer, SIGNAL(timerExpired(QString)), this, SLOT(showTimerExpired(QString)));
    connect(timer, SIGNAL(timerAlreadyExpired(QString)), this, SLOT(showTimerAlreadyExpired(QString)));

    //set threadlist
    validlist = new QList<QStringList>();

    //set trayicon actions
    reloadTrayIcon();

}

SystemTray::~SystemTray()
{
    delete builder;
    delete timer;
    delete validlist;
    delete trayIconMenu;
    delete trayIcon;
}

void SystemTray::show()
{
    trayIcon->show();
}

void SystemTray::hide()
{
    trayIcon->hide();
}

void SystemTray::onCheckStateChanged(bool checked)
{
     QString objname = this->sender()->objectName();
     int itemid = -1;
     int xmlid = -1;

     for(int i = 0; i < validlist->count(); i++){
         if(objname == validlist->at(i).at(1)) itemid = i;
     }

     if(itemid == -1) return;

     QStringList tmp = validlist->at(itemid);

     QString xmlitemid = tmp.at(0);
     xmlid = xmlitemid.toInt();

     if(checked){
         //append validlist
         tmp.removeAt(2);
         tmp.append(timer->addTimer(xmlid));
         showTimerStart(tmp.at(2));
     }else{
         timer->removeTimer(tmp.at(2));

         //show timer
         showTimerStopped(tmp.at(2));
         //reset validlist
         tmp.removeAt(2);
         tmp.append("");
     }

     //change xml data
     changeState(itemid);

     validlist->removeAt(itemid);
     validlist->insert(itemid, tmp);
}

void SystemTray::launchMainAction()
{
    emit launchMain();
}

void SystemTray::launchSettingsAction()
{
    emit launchSetting();
}

void SystemTray::trayCloseAction()
{
    emit launchclose();
}

void SystemTray::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        launchMainAction();
        break;
    default:
        break;
    }
}

void SystemTray::showTimerStart(QString threadname)
{
    //search valid data
    int itemid = getActionIdFromThread(threadname);

    // load setting datas
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    bool hidestarted = settings.value("HIDETIMERSTART", false).toBool();
    int timerms = settings.value("TIMERSTARTMS", 2500).toInt();
    settings.endGroup();


    if(itemid < 0 || hidestarted) return;

    qDebug() << "(timer started)";

    //get newtime from xml
    QList<QStringList> *list = new QList<QStringList>();

    QString textdata;
    //maybe slow
    if(builder->readItem(itemid, list)){
        QDateTime time = timer->getScheduledTime(threadname);
        textdata.append(list->at(0).at(1));
        textdata.append("\r\n");
        textdata.append(time.toString("yyyy/MM/dd HH:mm:ss"));
        textdata.append(" ");
        textdata.append(encodeDayOfWeek(time.date().dayOfWeek()));

    }
    //show message
    trayIcon->showMessage(tr("タイマーを開始しました"), textdata\
                          , QSystemTrayIcon::Information\
                          , timerms);
}

void SystemTray::showTimerStopped(QString threadname)
{
    //search valid data
    int itemid = getActionIdFromThread(threadname);

    if(itemid < 0) return;

    qDebug() << "(timer stopped)";

    //get newtime from xml
    QList<QStringList> *list = new QList<QStringList>();

    //maybe slow
    if(builder->readItem(itemid, list)){
        //show message
        trayIcon->showMessage(tr("タイマーを解除しました"),\
                              list->at(0).at(1),\
                              QSystemTrayIcon::Information,\
                              3500);
    }
}

void SystemTray::showTimerAlreadyExpired(QString threadname)
{
    //search valid data
    int itemid = getActionIdFromThread(threadname);

    if(itemid < 0) return;

    qDebug() << "(timer already expired)";

    //get newtime from xml
    QList<QStringList> *list = new QList<QStringList>();

    //maybe slow
    if(builder->readItem(itemid, list)){
        //show message
        trayIcon->showMessage(tr("期限切れです"),\
                              list->at(0).at(1),\
                              QSystemTrayIcon::Information,\
                              3500);

        //delete thread
        timer->removeTimer(threadname);

        //change checkstate
        changeState(itemid);
    }
}

void SystemTray::showTimerExpired(QString threadname)
{
    //search valid data
    int itemid = getActionIdFromThread(threadname);

    // load setting datas
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    bool hidestarted = settings.value("HIDETASKSTART", false).toBool();
    int timerms = settings.value("TASKSTARTMS", 2500).toInt();
    settings.endGroup();

    if(itemid < 0 || hidestarted) return;

    qDebug() << "(timer expired)";

    //get newtime from xml
    QList<QStringList> *list = new QList<QStringList>();

    //maybe slow
    if(builder->readItem(itemid, list)){
        //show message
        trayIcon->showMessage(tr("タスクを開始します"), list->at(0).at(1)
                              , QSystemTrayIcon::Information
                              , timerms);
    }
}

void SystemTray::setDynamicActionList()
{
    int count = builder->count();
    if(count > 0) trayIconMenu->addSeparator();

    for(int i = 0; i < count; i++){
        setDynamicAction(i);
    }
}

void SystemTray::reloadTrayIcon()
{
    //disconnect action
//    disconnect(launchAction, &QAction::triggered, this, &SystemTray::launchMainAction);
//    disconnect(settingsAction, &QAction::triggered, this, &SystemTray::launchSettingsAction);
//    disconnect(quitAction, &QAction::triggered, this, &QObject::deleteLater);

    launchAction = trayIconMenu->addAction(tr("メインウィンドウを開く…"));
    settingsAction = trayIconMenu->addAction(tr("設定を開く…"));

    //set list from profilexml
    setDynamicActionList();

    //add variant item
    trayIconMenu->addSeparator();
    quitAction = trayIconMenu->addAction(tr("終了"));

    connect(launchAction, &QAction::triggered, this, &SystemTray::launchMainAction);
    connect(settingsAction, &QAction::triggered, this, &SystemTray::launchSettingsAction);
    connect(quitAction, &QAction::triggered, this, &SystemTray::trayCloseAction);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    trayIcon->setContextMenu(trayIconMenu);
}

void SystemTray::setDynamicAction(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    QAction *dyact;
    if(builder->readItem(itemid, list)){
        dyact = new QAction(list->at(0).at(1), this);
        bool isvalid = (list->at(2).at(1) == "yes")? true : false;

        //set checked action
        dyact->setCheckable(true);
        dyact->setChecked(isvalid);

        //set object name
        dyact->setObjectName(list->at(8).at(1));

        //start scheduler if checkbox is valid
        QString threadname = isvalid ? timer->addTimer(itemid) : "";
        validlist->append((QStringList() << QString::number(itemid) << dyact->objectName() << threadname));

        connect(dyact, SIGNAL(triggered(bool)), this, SLOT(onCheckStateChanged(bool)));
        trayIconMenu->addAction(dyact);
    }

    delete list;
}

void SystemTray::changeState(int itemid)
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
    }

    delete list;
}

int SystemTray::getActionIdFromThread(QString threadname)
{
    //search valid data
    int itemid = -1;
    int count = validlist->count();
    for(int i = 0; i < count; i++){
        if(threadname == validlist->at(i).at(2)) itemid = i;
    }

    return itemid;
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
