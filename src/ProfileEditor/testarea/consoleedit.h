#ifndef CONSOLEEDIT_H
#define CONSOLEEDIT_H

#include <QLineEdit>
#include <runtasksignalbinder.h>

class ConsoleEdit : public QLineEdit
{
public:
    explicit ConsoleEdit(QWidget *parent = nullptr);
    ~ConsoleEdit();

    void setRunTaskSignalBinder(RunTaskSignalBinder *rbinder);

private slots:
    void sendMessage();

private:
    RunTaskSignalBinder *binder;

};

#endif // CONSOLEEDIT_H
