/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
     * @fn applicationVariant
     * @brief Get variables given from AutoBatchRunner etc.
     *        The type to be obtained is determined by InputType.
     *
     *        Global : QHash<QString, QString>
     *        Local  : QHash<QString, QString>
     *        File   : QStringList
     *e
     * @param variant global, local and file
     */
    void applicationVariant(PluginVariant *variant){
        getVariant(variant);
    }


};

Q_DECLARE_INTERFACE(ExtraPluginInterface, "abr.ExtraPluginInterface/1.0")

#endif // EXTRAPLUGININTERFACE_H
