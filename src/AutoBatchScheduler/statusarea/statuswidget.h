#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QDesktopWidget>
#include <taskschedulerconnector.h>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();

    void setTaskSchedulerConnector(TaskSchedulerConnector *task);
    void showWidget();
signals:
    void launchSettings();

public slots:
    void settingsClicked(){emit launchSettings();}

private:
    void closeEvent(QCloseEvent *event);

    Ui::StatusWidget *ui;
    TaskSchedulerConnector *taskc;
};

#endif // STATUSWIDGET_H
