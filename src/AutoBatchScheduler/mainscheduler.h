#ifndef MAINSCHEDULER_H
#define MAINSCHEDULER_H

#include <QMainWindow>

namespace Ui {
class MainScheduler;
}

class MainScheduler : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScheduler(QWidget *parent = 0);
    ~MainScheduler();

private:
    Ui::MainScheduler *ui;
};

#endif // MAINSCHEDULER_H
