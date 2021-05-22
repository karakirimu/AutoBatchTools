/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef INNERFUNCTIONS_H
#define INNERFUNCTIONS_H

#include "pluginvariants.h"

class InnerFunctions : public QObject
{
    Q_OBJECT
public:
    explicit InnerFunctions(){}
    virtual ~InnerFunctions() override {}

    /**
     * @fn getMessage
     * @brief Get message
     * @param type Message type.
     * @return If the invoked message type contains a string, return the message.
     */
    const QString getMessage(MessageType type){
        if(!messages.contains(static_cast<int>(type))) return "";
        return messages.value(static_cast<int>(type));
    }

    /**
     * @fn getInformation
     * @brief Return the set information to AutoBatchEditor etc.
     * @return PluginInformation structure
     */
    const PluginInformation *getInformation(){return &plugininfo;}

    /**
     * @overload
     * @fn setValue
     * @brief A function that provides variables from the executing process.
     * @param variant QHash
     * @param type Input type
     */
    void setValue(QHash<QString, QString> *variant, InputType type){
        switch (type) {
        case InputType::Local:
            pluginvariant.localvar = variant;
            break;
        case InputType::Global:
            pluginvariant.globalvar = *variant;
            break;
        case InputType::File:
            break;
        }
    }

    /**
     * @overload
     * @fn setValue
     * @brief A function that provides variables from the executing process.
     * @param variant QStringList
     * @param type Input type
     */
    void setValue(QStringList *variant, InputType type){
        switch (type) {
        case InputType::Local:
        case InputType::Global:
            break;
        case InputType::File:
            pluginvariant.filevar = variant;
            break;
        }
    }

signals:
    void updateMessage(QString);

protected:

    void setMessage(QString message, MessageType type){
        if(MessageType::Update == type){
            emit updateMessage(message);
        }else{
            InnerFunctions::messages.insert(static_cast<int>(type), message);
        }
    }

    void setInformation(PluginInformation info){
        InnerFunctions::plugininfo = info;
    }

    void getVariant(PluginVariant *variant){
        variant = &pluginvariant;
    }

private:
    QHash<int, QString> messages;
    PluginVariant pluginvariant;
    PluginInformation plugininfo;

};

#endif // INNERFUNCTIONS_H
