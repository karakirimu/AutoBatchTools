#ifndef MAINSCHEDULER_H
#define MAINSCHEDULER_H

#include "taskschedulerconnector.h"

#include <QMainWindow>
#include <QMouseEvent>
#include <QSettings>
#include <systemtray.h>

namespace Ui {
class MainScheduler;
}

class MainScheduler : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScheduler(QWidget *parent = 0);
    ~MainScheduler();

private slots:
    void onSchedulerButtonClicked();
    void onOptionButtonClicked();
    void showWindow();

    void closeEvent(QCloseEvent *event);

private:

    Ui::MainScheduler *ui;
    SystemTray *sysTray;
    TaskSchedulerConnector *taskscheduler;
};

#endif // MAINSCHEDULER_H
