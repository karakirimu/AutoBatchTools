#ifndef CONSOLEEDIT_H
#define CONSOLEEDIT_H

#include <QLineEdit>
#include <multitask.h>

class ConsoleEdit : public QLineEdit
{
public:
    explicit ConsoleEdit(QWidget *parent = nullptr);
    ~ConsoleEdit();

    void setMultiTask(MultiTask *rbinder);

private slots:
    void sendMessage();

private:
    MultiTask *binder;

};

#endif // CONSOLEEDIT_H
