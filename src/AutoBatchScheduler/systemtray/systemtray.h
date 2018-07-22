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
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();

    void showNotCloseMessage();
    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

    void show();
    void hide();

signals:
    void launchSetting();
    void launchclose();

private slots:
    //check state change from user
//    void onCheckStateChanged(bool checked);
    //check state change received from xml state
//    void updateCheckStateChanged(QString objname);

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

    //from table
//    void addlistAction(int xmlitemid);
//    void deletelistAction(QString objectname);

private:
    void initTrayIcon();
//    void initDynamicActionList();
//    void initDynamicAction(int itemid);
    void changeXmlValidState(int itemid);
    int getStartupXmlIndex(QString objectname);
    QString getNameByActions(QString objectname);
    QString encodeDayOfWeek(int dayofweek);
    void themeChangeAction();
//    QAction *generateAction(int itemid);
//    QSettings settings;

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
