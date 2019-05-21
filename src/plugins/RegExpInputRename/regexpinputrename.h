#ifndef REGEXPINPUTRENAME_H
#define REGEXPINPUTRENAME_H

#include "regexpinputrename_global.h"
#include "../ExtraPluginInterface/extraplugininterface.h"
#include <regexpinputrenamedialog.h>
#include <QFileInfo>
#include <QDir>
#include <QFile>

class REGEXPINPUTRENAMESHARED_EXPORT RegExpInputRename
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.RegExpInputRename" FILE "RegExpInputRename.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    RegExpInputRename(){
        pinfo.name = "RegExpInputRename";
        pinfo.version = "v 1.0.0";
        pinfo.author = "karakirimu";
        pinfo.tooltip = "When converting with multiple regular expressions, \n"
                        "the regular expression and the replacement character \n"
                        "are divided according to the characters specified in \n"
                        "the next text box, and character conversion is performed \n"
                        "from the forward.";

        pinfo.issettingwidget = true;
    }

     ~RegExpInputRename(){}

     int functionMain(int argc, QStringList *args);

     int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                             , QPoint parentpos, QString parentstylesheet);
};

#endif // REGEXPINPUTRENAME_H
