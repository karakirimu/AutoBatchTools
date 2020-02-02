/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EXTRAPLUGININTERFACE_H
#define EXTRAPLUGININTERFACE_H

//#include "extraplugininterface_global.h"
#include <QHash>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QtPlugin>

typedef struct {
    QString name;           //set plugin name
    QString version;        //set function's version　(preffered: v X.X.X)
    QString author;         //set yours name
    QString tooltip;        //This function sets explain string.
    bool issettingwidget;   //This function set true if you use setting widget function

} PLUGININFO;

class ExtraPluginInterface : public QObject
{
public:
    virtual ~ExtraPluginInterface(){}

    //This is plugin main function. return number 0 stands for success
    //Other numbers (e.g. -1) stands for failure.
    // int argc         : argument count
    // QStringList args : arguments
    virtual int functionMain(int argc, QStringList *args) = 0;

    //This function sets explain string.
//    virtual QString tooltipString() = 0;

    //set your function's version　(preffered: v X.X.X)
//    virtual QString version() = 0;

    //set your displayed name
//    virtual QString vendor() = 0;

    //This function set true if you use setting widget function
//    virtual bool existsSettingWidget() = 0;

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

    const PLUGININFO pluginInfo(){return pinfo;}

protected:
    void setErrorMessage(const QString message){ errorMessage = message; }
    void setSuccessMessage(const QString message){ successMessage = message; }

    QString errorMessage = "";
    QString successMessage = "";

    QHash<QString, QString> *local;
    QHash<QString, QString> global;
    QStringList file;

    // Describe the name, version, etc. in this structure after interface inheritance.
    PLUGININFO pinfo;

};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "abr.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
