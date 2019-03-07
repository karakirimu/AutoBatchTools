#ifndef BUFFEREDCOPY_H
#define BUFFEREDCOPY_H

#include "bufferedcopy_global.h"
#include "../ExtraPluginInterface/extraplugininterface.h"
#include "bufferedcopydialog.h"
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>

class BUFFEREDCOPYSHARED_EXPORT BufferedCopy
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.BufferedCopy" FILE "bufferedcopy.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
     ~BufferedCopy(){}

     int functionMain(int argc, QStringList *args);

     QString tooltipString(){ return ""; }

     bool existsSettingWidget(){return true;}

     int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                             , QPoint parentpos, QString parentstylesheet);

private:
     bool copyData(QString source, QString dest, long cachesize);
     bool checkHash(QString source, QString dest);

     QByteArray sha1Hash(QString file);

};

#endif // BUFFEREDCOPY_H
