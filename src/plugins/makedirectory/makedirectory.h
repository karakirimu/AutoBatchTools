#ifndef MAKEDIRECTORY_H
#define MAKEDIRECTORY_H

#include "makedirectory_global.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QDir>
#include <QPoint>
#include <QFileInfo>

class MAKEDIRECTORYSHARED_EXPORT MakeDirectory
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.MakeDirectory" FILE "makedirectory.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    MakeDirectory(){
        pinfo.name = "MakeDirectory";
        pinfo.version = "v 1.0.0";
        pinfo.author = "karakirimu";
        pinfo.tooltip = "Create all the directories needed for the folder path.\n"
                        "argument 1 : The folder path you want to create";
        pinfo.issettingwidget = false;
    }

    ~MakeDirectory(){}

    int functionMain(int argc, QStringList *args);

    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                            , QPoint parentpos, QString parentstylesheet);
};

#endif // MAKEDIRECTORY_H
