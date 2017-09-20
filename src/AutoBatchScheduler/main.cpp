#include "mainscheduler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScheduler w;

    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    if(settings.value("HIDEINWDOW", false).toBool()){
        w.show();
    }
    settings.endGroup();
    return a.exec();
}
