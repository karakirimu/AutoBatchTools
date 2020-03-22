#ifndef REMOVEDIRECTORY_H
#define REMOVEDIRECTORY_H

#include "removedirectory_global.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QDir>
#include <QPoint>
#include <QFileInfo>
#include <removedirectorydialog.h>
#include <QDirIterator>

class REMOVEDIRECTORYSHARED_EXPORT RemoveDirectory
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.RemoveDirectory" FILE "removedirectory.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    RemoveDirectory(){
        pinfo.name = "RemoveDirectory";
        pinfo.version = "v 1.0.0";
        pinfo.author = "karakirimu";
        pinfo.tooltip = "argument 1 : The folder path you want to remove\n"
                        "argument 2 : Whether to delete files in folder (-rf)";
        pinfo.issettingwidget = true;
    }

    ~RemoveDirectory(){}

    int functionMain(int argc, QStringList *args);

    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                            , QPoint parentpos, QString parentstylesheet);
};

#endif // REMOVEDIRECTORY_H
