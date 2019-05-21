#ifndef SLEEP_H
#define SLEEP_H

#include "sleep_global.h"
#include "../ExtraPluginInterface/extraplugininterface.h"
#include <sleepdialog.h>
#include <QEventLoop>
#include <QTimer>

class SLEEPSHARED_EXPORT Sleep
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.Sleep" FILE "sleep.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    Sleep(){
        pinfo.name = "Sleep";
        pinfo.version = "v 1.0.0";
        pinfo.author = "karakirimu";
        pinfo.tooltip = "";
        pinfo.issettingwidget = true;
    }

     ~Sleep(){}

     int functionMain(int argc, QStringList *args);

     int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                             , QPoint parentpos, QString parentstylesheet);

};

#endif // SLEEP_H
