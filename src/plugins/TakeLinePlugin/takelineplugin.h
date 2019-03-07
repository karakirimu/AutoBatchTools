#ifndef TAKELINEPLUGIN_H
#define TAKELINEPLUGIN_H

#include "takelineplugin_global.h"
#include "../ExtraPluginInterface/extraplugininterface.h"
#include "takelinedialog.h"

#include <QFile>
#include <QTextStream>

class TAKELINEPLUGINSHARED_EXPORT TakeLinePlugin
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.TakeLinePlugin" FILE "takelineplugin.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    ~TakeLinePlugin(){}

    int functionMain(int argc, QStringList *args);

    QString tooltipString(){
        return tr("This function can take line 'n'.\n"
                  "arguments 1 : filename or string with '\\n'\n"
                  "arguments 2 : line number(it starts 0)\n"
                  "arguments 3 : return local variant name");
    }

    bool existsSettingWidget(){return true;}

    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                            , QPoint parentpos, QString parentstylesheet);

private:
    enum{FILEORSTRING, LINENUM, VAR};
};

#endif // TAKELINEPLUGIN_H
