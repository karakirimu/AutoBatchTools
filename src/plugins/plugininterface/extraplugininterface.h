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


#include "InnerFunctions.h"

/**
 * @brief The ExtraPluginInterface class
 * Interface class for writing a plugin to customize AutoBatchRunner.
 */
class ExtraPluginInterface : public InnerFunctions
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

protected:

    /**
     * @fn functionMessage
     * @brief Sets the text displayed on the console, such as autobatchrunner.
     * @param message The message you want to display.
     * @param type Message type.
     */
    void functionMessage(QString message, MessageType type){
        setMessage(message, type);
    }

    /**
     * @fn functionInfo
     * @brief Set basic information for plugins.
     * @param info PluginInformation structure.
     */
    void functionInfo(PluginInformation info){
        setInformation(info);
    }

    /**
     * @fn functionWidgetSetting
     * @brief Function to reflect the settings of Widget to ProfileEditor.
     *        Not used when PluginInformation::issettingwidget is false.
     * @param args Argument list used in functionMain function.
     */
//    void functionWidgetSetting(QStringList args){
//        setWidgetMessage(&args);
//    }

    template<typename T>
    /**
     * @fn applicationVariant
     * @brief Get variables given from AutoBatchRunner etc.
     *        The type to be obtained is determined by InputType.
     *
     *        Global : QHash<QString, QString>
     *        Local  : QHash<QString, QString>
     *        File   : QStringList
     *
     * @param type enum class of Input type
     * @param variant
     */
    void applicationVariant(InputType type, T *variant){
        getVariant<T>(type, variant);
    }


};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "abr.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
