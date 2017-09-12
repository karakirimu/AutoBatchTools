#include "mainscheduler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScheduler w;
    w.show();

    return a.exec();
}
