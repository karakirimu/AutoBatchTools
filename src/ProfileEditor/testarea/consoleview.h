#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QWidget>
#include <runtasksignalbinder.h>
#include <../ConsoleBase/consolebase.h>

class ConsoleView : public ConsoleBase
{
    Q_OBJECT
public:
    explicit ConsoleView(QWidget *parent = nullptr);
    ~ConsoleView();

    //ui settings
    void setRunTaskSignalBinder(RunTaskSignalBinder *rbinder);

private:
    RunTaskSignalBinder *binder;
};

#endif // CONSOLEVIEW_H
