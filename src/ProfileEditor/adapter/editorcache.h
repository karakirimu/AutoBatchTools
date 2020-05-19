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

#ifndef EDITCACHE_H
#define EDITCACHE_H

#include <QHash>
#include <QString>
#include <QStringList>

/**
 * @brief The TAB enum
 *        Current function select tab position selection constant
 */
enum class TAB {

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

    enum TYPE{
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

/**
 * @brief The Information struct
 *        The initial value is the default value.
 */
struct Information {

    //! Profile name
    QString name = "";

    //! Profile version
    QString version = "";

    //! Profile author
    QString author = "";

    //! Profile description
    QString description = "";

    //! Whether to allow file input
    bool fileInput = true;

    //! Whether to use the file search result as the input file
    bool fileInputSearch = false;

    //! File search profile name when inputting file search results as a file
    QString fileSearchName = "";

    //! File search profile index when inputting file search results as a file
    int fileSearchIndex = -1;

    //! Whether to run all input files
    bool processAll = true;

    //! Input file maximum processing count
    int processMaxCount = 1;

    //! Number of input files used in one profile execution
    int argumentsInOneLoop = 1;

    //! Maximum number of self-call loops with profileload
    int recursiveLoopMax = 0;

};

/**
 * @brief The VariantPair class
 *        The initial value is the default value.
 */
class VariantPair {
public:
    explicit VariantPair(){}
    ~VariantPair(){}

    VariantPair(const QString &var, const QString &val){
        this->variant = var;
        this->value = val;
    }

    void operator =(const VariantPair &vp){
        variant = vp.variant;
        value = vp.value;
    }

    void clear(){
        variant = "";
        value = "";
    }

    //! Variant
    QString variant = "";

    //! Value
    QString value = "";
};

/**
 * @brief The Local struct
 *        The initial value is the default value.
 */
struct Local {

    // Number of variant (using only XML)
    // int variantCount = 0;

    //! Variant data
    QList<VariantPair> variantData;
};

/**
 * @brief The Execute struct
 *        The initial value is the default value.
 */
struct Execute {

    //! Launch scheduler operation only
    bool schedulerOnly = false;

    //! Timeout options enable or not
    bool timeoutEnabled = false;

    //! Timeout duration (ms)
    int timeout = 30000;

    //! Enable if you do not wait for the end of the executable file option
    bool detach = false;

    // Number of arguments (using only XML)
    // int commandCount = 0;

    //! Arguments data
    QStringList command;
};

/**
 * @brief The FileSearch struct
 *        The initial value is the default value.
 */
struct FileSearch {

    //! Launch scheduler operation only
    bool schedulerOnly = false;

    //! Combobox selected name
    QString name = "";

    //! Combobox selected index
    int nameIndex = -1;

    //! Search result list separator
    QString separator = "";

    //! Search result return selection constant (use outputOption)
    enum {
        //! 0
        VARIANT,

        //! 1
        FILEOUTPUT
    };

    //! Search result output options
    int outputOption = 0;

    //! Search result return variant name
    QString variant = "";

    //! Search result return output file path
    QString filePath = "";

    //! Search result output return file selection constant (use writeOption)
    enum {
        //! 0
        OVERWRITE,

        //! 1
        APPEND
    };

    //! Search result output return file options
    int writeOption = 0;
};

/**
 * @brief The Plugin struct
 *        The initial value is the default value.
 */
struct Plugin {

    //! launch scheduler operation only (plugin)
    bool schedulerOnly = false;

    //! Selected plugin name
    QString name = "";

    //! Selected plugin filepath
    QString filePath = "";

    // Number of arguments (using only XML)
    // int commandCount;

    //! Arguments data
    QStringList command;
};

/**
 * @brief The ProfileLoad struct
 *        The initial value is the default value.
 */
struct ProfileLoad {

    //! launch scheduler operation only (profileload)
    bool schedulerOnly = false;

    //! Selected profileload filepath
    QString filePath = "";
};

/**
 * @class The EditCache class
 * @brief Data storage class that reflects the data read from XML to
 *        GUI operation and the result of GUI operation to XML.
 */
class EditorCache
{
public:
    explicit EditorCache();

    EditorCache operator =(const EditorCache &ec){
        this->type = ec.type;
        this->functionSelect = ec.functionSelect;
        this->info = ec.info;
        this->local = ec.local;
        this->exec = ec.exec;
        this->filesearch = ec.filesearch;
        this->plugin = ec.plugin;
        this->profileload = ec.profileload;
        return *this;
    }

    // [shared]

    //! XML data type
    QString type = "";

    //! Current function select tab position
    int functionSelect = -1;

    // [information]
    Information info;

    // [local]
    Local local;

    // [execute]
    Execute exec;

    // [filesearch]
    FileSearch filesearch;

    // [plugin]
    Plugin plugin;

    // [profileload]
    ProfileLoad profileload;

};

#endif // EDITCACHE_H
