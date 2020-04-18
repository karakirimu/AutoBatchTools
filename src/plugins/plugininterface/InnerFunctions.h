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

#ifndef INNERFUNCTIONS_H
#define INNERFUNCTIONS_H

#include <QStringList>
#include <QHash>
#include "pluginvariants.h"

class InnerFunctions : public QObject
{
    Q_OBJECT
public:
    explicit InnerFunctions(){
//        widgetsettinglist = new QStringList();
        filevar = new QStringList();
    }

    ~InnerFunctions(){
//        delete widgetsettinglist;
        delete filevar;
    }

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
     * @brief Return the set information to ProfileEditor etc.
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
            InnerFunctions::localvar = variant;
            break;
        case InputType::Global:
            InnerFunctions::globalvar = *variant;
            break;
        case InputType::File:
            break;
//        case InputType::WidgetSetting:
//            break;
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
            filevar = variant;
            break;
//        case InputType::WidgetSetting:
//            widgetsettinglist = variant;
//            break;
        }
    }

signals:
    void updateMessage(QString);
//    void updateSetting(QStringList);

protected:

    void setMessage(QString message, MessageType type){
        if(MessageType::Update == type){
            emit updateMessage(message);
        }else{
            InnerFunctions::messages.insert(static_cast<int>(type), message);
        }
    }

//    void setWidgetMessage(QStringList *args){
//        emit updateSetting(*args);
//    }

    void setInformation(PluginInformation info){
        InnerFunctions::plugininfo = info;
    }

    template<typename T>
    void getVariant(InputType type, T *variant){
        Q_UNUSED(type)
        variant = nullptr;
    }

    template<>
    void getVariant(InputType type, QHash<QString, QString> *variant){
        variant = nullptr;
        switch (type) {
        case InputType::Local: variant = localvar; break;
        case InputType::Global: *variant = globalvar; break;
        case InputType::File: break;
//        case InputType::WidgetSetting: break;
        }
    }

    template<>
    void getVariant(InputType type, QStringList *variant){
        variant = nullptr;
        switch (type) {
        case InputType::Local: break;
        case InputType::Global: break;
        case InputType::File: variant = filevar; break;
//        case InputType::WidgetSetting: variant = widgetsettinglist; break;
        }
    }

private:

    QHash<int, QString> messages;

    QHash<QString, QString> *localvar;
    QHash<QString, QString> globalvar;
    QStringList *filevar;
    //QStringList *widgetsettinglist;

    //static PLUGININFO pinfo;
    PluginInformation plugininfo;

};

#endif // INNERFUNCTIONS_H
