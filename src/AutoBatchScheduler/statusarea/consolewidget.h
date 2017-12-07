#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QWidget>
#include "taskschedulerconnector.h"

namespace Ui {
class ConsoleWidget;
}

class ConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget *parent = 0);
    ~ConsoleWidget();

    void setConsoleViewTarget(TaskSchedulerConnector *taskc, QString objname);

private:
    Ui::ConsoleWidget *ui;
};

#endif // CONSOLEWIDGET_H
