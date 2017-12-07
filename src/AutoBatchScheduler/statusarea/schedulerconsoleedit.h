#ifndef SCHEDULERCONSOLEEDIT_H
#define SCHEDULERCONSOLEEDIT_H

#include <QLineEdit>
#include <taskschedulerconnector.h>

class SchedulerConsoleEdit : public QLineEdit
{
public:
    explicit SchedulerConsoleEdit(QWidget *parent = nullptr);
    ~SchedulerConsoleEdit();

    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

private slots:
    void sendMessage();

private:
    TaskSchedulerConnector *taskc;
};

#endif // SCHEDULERCONSOLEEDIT_H
