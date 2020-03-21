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

#include <QHash>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QtPlugin>

typedef struct {
    //! plugin name
    QString name;

    //! functions version (preferred: v X.X.X)
    QString version;

    //! author name
    QString author;

    //! explain string
    QString tooltip;

    //! true if you use setting widget
    bool issettingwidget;

} PLUGININFO;

/**
 * @brief The ExtraPluginInterface class
 * Interface class for writing a plugin to customize AutoBatchRunner.
 */
class ExtraPluginInterface : public QObject
{
public:
    virtual ~ExtraPluginInterface(){}

    /**
     * @fn functionMain
     * @brief This is plugin main function.
     * @param argc arguments count
     * @param args arguments
     * @return Returns 0 on success. Otherwise, -1.
     */
    virtual int functionMain(int argc, QStringList *args) = 0;

    /**
     * @fn launchSettingWidget
     * @brief This function is called above function returns true.
     *        currentargs represents input, and resultargs represents output.
     *
     * @param currentargs
     * @param resultargs
     * @param parentpos
     * @param parentstylesheet
     * @return It returns argument of tablewidget
     */
    virtual int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                                    , QPoint parentpos, QString parentstylesheet = "") = 0;

    //This is signal. It can send message to main console view
//    virtual void sendMessage(const QString message) = 0;

    //In case of error termination, call this function and get the set character string.
    QString functionErrorMessage(){ return errorMessage; }
    QString functionSuccessMessage(){ return successMessage; }

    //Used when executing, rewriting data.
    void setLocalValue(QHash<QString, QString> *local){ this->local = local; }
    void setGlobalValue(QHash<QString, QString> global){this->global= global;}
    void setInputFileData(QStringList file){this->file  = file;  }

    /**
     * @fn pluginInfo
     * @brief Return the set information to ProfileEditor etc.
     * @return PLUGININFO structure
     */
    const PLUGININFO pluginInfo(){return pinfo;}

protected:
    void setErrorMessage(const QString message){ errorMessage = message; }
    void setSuccessMessage(const QString message){ successMessage = message; }

    QString errorMessage = "";
    QString successMessage = "";

    QHash<QString, QString> *local;
    QHash<QString, QString> global;
    QStringList file;

    PLUGININFO pinfo;

};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "abr.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
