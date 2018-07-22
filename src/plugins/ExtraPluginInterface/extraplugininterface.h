#ifndef EXTRAPLUGININTERFACE_H
#define EXTRAPLUGININTERFACE_H

#include <QHash>
#include <QStringList>
#include <QObject>
#include <QtPlugin>
//#include "runnerextraplugininterface_global.h"

class /*RUNNEREXTRAPLUGININTERFACESHARED_EXPORT*/ ExtraPluginInterface /*: public QObject*/
{
//    Q_OBJECT
public:
    ExtraPluginInterface();
    virtual ~ExtraPluginInterface(){}

    //This is plugin main function. return number 0 stands for success

    //other number stands for failure.
    virtual int functionMain(int argc, QStringList *args) = 0;

    //This function sets explain string.
    virtual QString getDetailString() = 0;

    //This function set true if you use setting widget function
    virtual bool existsSettingWidget() = 0;

    //This function is called above function returns true.
    //It returns argument of tablewidget
    virtual int launchSettingWidget(QStringList *resultargs) = 0;

    //this returns subclass object
//    virtual QObject *getObject() = 0;

    //There are called in launch data.
    void setLocalValue(QHash<QString, QString> *local){ this->local = local; }
    void setGlobalValue(QHash<QString, QString> global){this->global= global;}
    void setInputFileData(QStringList file){this->file  = file;  }

//signals:
//    //This signal can send message to main console view
//    virtual void sendMessage(QString) = 0;

protected:
    QHash<QString, QString> *local;
    QHash<QString, QString> global;
    QStringList file;

};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "autobatch.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
