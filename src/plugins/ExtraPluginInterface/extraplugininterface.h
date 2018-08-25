#ifndef EXTRAPLUGININTERFACE_H
#define EXTRAPLUGININTERFACE_H

#include <QHash>
#include <QStringList>
#include <QObject>
#include <QtPlugin>
//#include "extraplugininterface_global.h"

class ExtraPluginInterface
{

public:
    virtual ~ExtraPluginInterface(){}


    //This is signal. It can send message to main console view
    virtual void sendMessage(QString) = 0;

    //This is plugin main function. return number 0 stands for success
    //other number stands for failure.
    virtual int functionMain(int argc, QStringList *args) = 0;


    //This function sets explain string.
    virtual QString getDetailString() = 0;


    //This function set true if you use setting widget function
    virtual bool existsSettingWidget() = 0;


    //This function is called above function returns true.
    //currentargs represents input, and resultargs represents output.
    //It returns argument of tablewidget
    virtual int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                                    , QPoint parentpos, QString parentstylesheet = "") = 0;


    //Used when executing, rewriting data.
    void setLocalValue(QHash<QString, QString> *local){ this->local = local; }
    void setGlobalValue(QHash<QString, QString> global){this->global= global;}
    void setInputFileData(QStringList file){this->file  = file;  }

protected:
    QHash<QString, QString> *local;
    QHash<QString, QString> global;
    QStringList file;

};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "autobatch.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
