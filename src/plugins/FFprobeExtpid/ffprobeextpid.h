#ifndef FFPROBEEXTPID_H
#define FFPROBEEXTPID_H

#include "ffprobeextpid_global.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include "ffprobeextpiddialog.h"

class FFPROBEEXTPIDSHARED_EXPORT FFprobeExtpid
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.FFprobeExtpid" FILE "FFprobeExtpid.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    FFprobeExtpid(){
        pinfo.name = "FFprobeExtpid";
        pinfo.version = "v 1.0.0";
        pinfo.author = "karakirimu";
        pinfo.tooltip = "";
        pinfo.issettingwidget = true;
    }

    ~FFprobeExtpid(){}

    int functionMain(int argc, QStringList *args);

    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                            , QPoint parentpos, QString parentstylesheet);

private:
    bool writeToText(QString filename, QString text);
};

#endif // FFPROBEEXTPID_H
