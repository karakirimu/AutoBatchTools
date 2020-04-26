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
 * @brief The VariantPair struct
 *        The initial value is the default value.
 */
struct VariantPair {

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
    ~EditorCache();

    // [shared]

    //! XML data type
    QString type = "";

    //! Current function select tab position selection constant
    enum {

        //! 0
        EXECUTE,

        //! 1
        SEARCH,

        //! 2
        PLUGINS,

        //! 3
        PROFILELOAD
    };

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