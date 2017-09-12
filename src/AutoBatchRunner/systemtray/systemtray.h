#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QSystemTrayIcon>
#include <startupxmlbuilder.h>
#include <timemanager.h>

/**
 * @brief The SystemTray class
 * @param QList<QStringList> *threadlist structure
 * Left Listnum:
 * Top  StringListnum:
 * \    :      0:                1:          2:
 * 0    :itemid :generatedobjname :threadname :
 * 1    :itemid :generatedobjname :threadname :
 * it follows next
 * .
 * .
 */

class SystemTray : public QWidget
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();

    void show();
    void hide();
signals:
    void launchMain();
    void launchSetting();
    void launchclose();

private slots:
    void onCheckStateChanged(bool checked);
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void showTimerStart(QString threadname);
    void showTimerStopped(QString threadname);
    void showTimerAlreadyExpired(QString threadname);
    void showTimerExpired(QString threadname);
    void launchMainAction();
    void launchSettingsAction();
    void trayCloseAction();

private:
    void reloadTrayIcon();
    void setDynamicActionList();
    void setDynamicAction(int itemid);
    void changeState(int itemid);
    int getActionIdFromThread(QString threadname);
    QString encodeDayOfWeek(int dayofweek);

    QMenu *trayIconMenu;
    QAction *launchAction;
    QAction *settingsAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;

    StartupXmlBuilder *builder;
    TimeManager *timer;
    QList<QStringList> *validlist;

};

#endif // SYSTEMTRAY_H
