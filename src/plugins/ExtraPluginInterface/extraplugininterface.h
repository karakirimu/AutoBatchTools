#ifndef EXTRAPLUGININTERFACE_H
#define EXTRAPLUGININTERFACE_H

//#include "extraplugininterface_global.h"
#include <QHash>
#include <QStringList>
#include <QObject>
#include <QtPlugin>

class ExtraPluginInterface : public QObject
{
public:
    virtual ~ExtraPluginInterface(){}

    //This is plugin main function. return number 0 stands for success
    //Other numbers (e.g. -1) stands for failure.
    virtual int functionMain(int argc, QStringList *args) = 0;

    //This function sets explain string.
    virtual QString tooltipString() = 0;

    //set your function's version　(preffered: v X.X.X)
    virtual QString version() = 0;

    //set your displayed name
    virtual QString vendor() = 0;

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

    //This is signal. It can send message to main console view
//    virtual void sendMessage(const QString message) = 0;

    //In case of error termination, call this function and get the set character string.
    QString functionErrorMessage(){ return errorMessage; }


    QString functionSuccessMessage(){ return successMessage; }

protected:
    void setErrorMessage(const QString message){ errorMessage = message; }
    void setSuccessMessage(const QString message){ successMessage = message; }

    QString errorMessage = "";
    QString successMessage = "";

    QHash<QString, QString> *local;
    QHash<QString, QString> global;
    QStringList file;

};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "abr.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
