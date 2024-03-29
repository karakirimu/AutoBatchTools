/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef FUNCTIONTYPE_H
#define FUNCTIONTYPE_H

#include <QString>

/**
 * @brief The TAB enum
 *        Current function select tab position selection constant
 */
enum class TAB : int {

    //! -1
    INVALID = -1,

    //! 0
    EXECUTE,

    //! 1
    FILESEARCH,

    //! 2
    PLUGINS,

    //! 3
    PROFILELOAD
};

/**
 * @brief XML load type selection
 */
class FunctionType{
public:
    explicit FunctionType(){}

    enum TYPE : int {
        //! AllInclude list (This type contains EXECUTE,FILESEARCH,PLUGIN,PROFILELOAD)
        ALLINCLUDE,

        //! Information list
        INFORMATION,

        //! Local list
        LOCAL,

        //! Execute list
        EXECUTE,

        //! FileSearch list
        FILESEARCH,

        //! Plugin list
        PLUGIN,

        //! ProfileLoad list
        PROFILELOAD,

        //! Invalid paramater
        INVALID
    };

    TYPE getType(QString type){
        if(type == VALUE_TYPEALL)    return TYPE::ALLINCLUDE;
        if(type == VALUE_TYPEEXEC)   return TYPE::EXECUTE;
        if(type == VALUE_TYPESEARCH) return TYPE::FILESEARCH;
        if(type == VALUE_TYPEPLUGIN) return TYPE::PLUGIN;
        if(type == VALUE_TYPEPLOAD)  return TYPE::PROFILELOAD;
        if(type == VALUE_TYPEINFO)   return TYPE::INFORMATION;
        if(type == VALUE_TYPELOCAL)  return TYPE::LOCAL;

        return TYPE::INVALID;
    }

    TYPE getType(TAB type){
        if(type == TAB::EXECUTE)     return TYPE::EXECUTE;
        if(type == TAB::FILESEARCH)  return TYPE::FILESEARCH;
        if(type == TAB::PLUGINS)     return TYPE::PLUGIN;
        if(type == TAB::PROFILELOAD) return TYPE::PROFILELOAD;

        return TYPE::INVALID;
    }

    QString getString(TYPE type){
        switch (type) {
        case TYPE::ALLINCLUDE:  return VALUE_TYPEALL;
        case TYPE::INFORMATION: return VALUE_TYPEINFO;
        case TYPE::LOCAL:       return VALUE_TYPELOCAL;
        case TYPE::EXECUTE:     return VALUE_TYPEEXEC;
        case TYPE::FILESEARCH:  return VALUE_TYPESEARCH;
        case TYPE::PLUGIN:      return VALUE_TYPEPLUGIN;
        case TYPE::PROFILELOAD: return VALUE_TYPEPLOAD;
        case TYPE::INVALID:     return "";
        }

        return "";
    }

private:

    //! Shared Value
    const QString VALUE_TYPEINFO              = "info";
    const QString VALUE_TYPEEXEC              = "normal";
    const QString VALUE_TYPESEARCH            = "search";
    const QString VALUE_TYPEPLUGIN            = "script";
    const QString VALUE_TYPEPLOAD             = "other";

    const QString VALUE_TYPEALL               = "temp";
    const QString VALUE_TYPELOCAL             = "local";

};

#endif // FUNCTIONTYPE_H
