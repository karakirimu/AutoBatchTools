#ifndef CONSOLEEDIT_H
#define CONSOLEEDIT_H

#include <QLineEdit>
#include <multitaskp.h>

class ConsoleEdit : public QLineEdit
{
public:
    explicit ConsoleEdit(QWidget *parent = nullptr);
    ~ConsoleEdit();

    void setMultiTask(MultiTaskP *rbinder);

private slots:
    void sendMessage();

private:
    MultiTaskP *binder;

};

#endif // CONSOLEEDIT_H
