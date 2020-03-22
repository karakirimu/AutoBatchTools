#ifndef FILETEXTREPLACE_H
#define FILETEXTREPLACE_H

#include "filetextreplace_global.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <filetextreplacedialog.h>

class FILETEXTREPLACESHARED_EXPORT FileTextReplace
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.FileTextReplace" FILE "FileTextReplace.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    FileTextReplace(){
        pinfo.name = "FileTextReplace";
        pinfo.version = "v 1.0.0";
        pinfo.author = "karakirimu";
        pinfo.tooltip = "";
        pinfo.issettingwidget = true;
    }

    ~FileTextReplace(){}

    int functionMain(int argc, QStringList *args);

    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                            , QPoint parentpos, QString parentstylesheet);

private:
    bool writeToText(QString filename, QString text);

};

#endif // FILETEXTREPLACE_H
