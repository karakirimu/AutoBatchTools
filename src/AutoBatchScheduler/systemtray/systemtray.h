#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QSystemTrayIcon>
#include <startupxmlbuilder.h>
#include <statuswidget.h>
#include <taskschedulerconnector.h>

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
    void onCheckStateChanged(bool checked);
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void showTimerStart(QString objectname, QDateTime time);
    void showTimerStopped(QString objectname, int type);
    void showProcessStart(QString objectname, int runfrom);
    void showProcessPause(QString objectname);
    void showProcessStopped(QString objectname);
    void showProcessEnded(QString objectname, int type);
    void showTaskDisabled(QString objectname);
    void launchSettingsAction();
    void trayCloseAction();

    //from table
    void addlistAction(int xmlitemid);
    void deletelistAction(QString objectname);

private:
    void initTrayIcon();
    void initDynamicActionList();
    void initDynamicAction(int itemid);
    void changeXmlValidState(int itemid);
    int getStartupXmlIndex(QString objectname);
    QString getNameByActions(QString objectname);
    QString encodeDayOfWeek(int dayofweek);
    QAction *generateAction(int itemid);

    QMenu *trayIconMenu;
    QAction *launchAction;
    QAction *settingsAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    StatusWidget *strw;

    StartupXmlBuilder *builder;
    TaskSchedulerConnector *taskc;

};

#endif // SYSTEMTRAY_H
