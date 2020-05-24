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

#ifndef PROCESSXMLLISTGENERATOR_H
#define PROCESSXMLLISTGENERATOR_H

#include "processxmllistgenerator_global.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

#include <QDebug>

//#include "processxmlelement.h"

/**
 * @brief The XmlListGenerator class
 * This class can create default list (to xml) and operate xml states.
 */
/**
 * @brief create list type arguments of QStringList
 * QStringList structure createInfoList
 * 0    :name_text  :
 * 1    :ver_text   :
 * 2    :author_text:
 * 3    :desc_text  :
 * 4    :finput_yesno:
 * 5    :sinput_yesno:
 * 6    :name_text   :
 * 7    :name_id_data:
 * 8    :rloop_yesno :
 * 9    :rloop_count :
 * 10   :rlarg_count :
 * 11   :reloop_count:
 *
 * QStringList structure createLocalList
 * 0    :localc_counts:
 * 1    :lvar_text    :
 * 2    :lval_text    :
 * -           *
 * -           *
 * -           *
 * - They follow to the end by turns.
 *
 */


// tag define (index 0)
// all

//! XML tag "type"
//#define ALL_TYPE                     "type"

//// info
//#define I_NAME                       "iname"
//#define I_VERSION                    "ver"
//#define I_AUTHOR                     "author"
//#define I_DESCRIPTION                "desc"
//#define I_FILEINPUT                  "finput"
//#define I_FILEINPUT_SEARCHCHECK      "sinput"
//#define I_FILESEARCH_NAME            "fsname"
//#define I_RECURSIVE_LOOP             "rloop"
//#define I_RECURSIVE_LOOPARGCOUNT     "rlarg"
//#define I_RECURSIVE_LOOPCOUNT        "reloop"

//// search
//#define S_NAME                       "sname"
//#define S_SEPARATOR                  "sep"
//#define S_VARIANT                    "var"
//#define S_OUTPUTFILE                 "output"
//#define S_OUTPUTFILETYPE             "fitype"

//// profile

//// XML tag "prname"
////#define PR_NAME                      "prname"
////! XML tag "prfile"
//#define PR_FILEPATH                  "prfile"

//// exec(normal)
//#define E_TIMEOUT                    "timeout"
//#define E_RUNDETACH                  "detach"
//#define E_CMD                        "exc"
//#define E_CMDARGCOUNT                "cmdc"

//// plugin(extrafunc)
//#define PL_NAME                      "plname"
//#define PL_FILEPATH                  "plfile"
//#define PL_CMD                       "plc"
//#define PL_CMDARGCOUNT               "pcmdc"

//// temp
//#define TE_STACKEDWIDGET_POSITION    "istack"

//// local
//#define L_VARIANT                    "lvar"
//#define L_VAR_COUNT                  "localc"

//// value define (index 1)
//#define TYPE_INFO                    "info"
//#define TYPE_EXEC                    "normal"
//#define TYPE_SEARCH                  "search"
//#define TYPE_SCRIPT                  "script"
//#define TYPE_ANOTHER                 "other"

//#define TYPE_ALLINCLUDE              "temp"
//#define TYPE_LOCAL                   "local"

//// attribute define (index 2)
//// all
//#define ATTR_ONLY_SCHEDULER          "only"
//#define ATTR_POSNUM                  "id"
//#define ATTR_RADIOBUTTONPOS          "radio"
//#define ATTR_MAXCOUNT                "max"
//#define ATTR_NONE                    ""

//// normal
//#define ATTR_TIMEOUTMS               "dur"

//// local
//#define ATTR_LOCALVALUE              "lval"

// undo redo operation command
//#define UNDOREDO_ADD                 "ADD"
//#define UNDOREDO_DELETE              "DEL"
//#define UNDOREDO_INSERT              "INS"
//#define UNDOREDO_SWAP                "SWA"
//#define UNDOREDO_MOVE                "MOV"
//#define UNDOREDO_EDIT                "EDI"

//#define UNDOREDO_E_TABLEADD          "ET_ADD"
//#define UNDOREDO_E_TABLEEDIT         "ET_EDIT"
//#define UNDOREDO_E_TABLEDEL          "ET_DEL"
//#define UNDOREDO_E_TABLEINS          "ET_INS"
//#define UNDOREDO_E_TABLESWAP         "ET_SWAP"
//#define UNDOREDO_E_TABLEMOVE         "ET_MOVE"

//#define UNDOREDO_PL_TABLEADD         "PLT_ADD"
//#define UNDOREDO_PL_TABLEEDIT        "PLT_EDIT"
//#define UNDOREDO_PL_TABLEDEL         "PLT_DEL"
//#define UNDOREDO_PL_TABLEINS         "PLT_INS"
//#define UNDOREDO_PL_TABLESWAP        "PLT_SWAP"
//#define UNDOREDO_PL_TABLEMOVE        "PLT_MOVE"
//#define UNDOREDO_PL_ALLUPDATE        "PLT_AUPD"

//#define UNDOREDO_LV_ADD              "LV_ADD"
//#define UNDOREDO_LV_EDIT             "LV_EDIT"
//#define UNDOREDO_LV_DEL              "LV_DEL"
//#define UNDOREDO_LV_INS              "LV_INS"
//#define UNDOREDO_LV_SWAP             "LV_SWAP"

//#define UNDOREDO_GV_ADD              "GV_ADD"
//#define UNDOREDO_GV_EDIT             "GV_EDIT"
//#define UNDOREDO_GV_DEL              "GV_DEL"
//#define UNDOREDO_GV_INS              "GV_INS"
//#define UNDOREDO_GV_SWAP             "GV_SWAP"

////undo redo table operation id value. It is no means.
////?
//#define E_ADD_TABLE      "{(I*9g-v"
//#define E_INSERT_TABLE   "M$7>O/Qb"
//#define E_EDIT_TABLE     "d/%o'q9l"
//#define E_DELETE_TABLE   "ht9!YQ(:"
//#define E_SWAP_TABLE     ",5Dm&y/R"

//#define PL_ADD_TABLE       ")r]&mwqn"
//#define PL_INSERT_TABLE    "8NGM:Yv3"
//#define PL_EDIT_TABLE      "UOq:>~`c"
//#define PL_DELETE_TABLE    "/=gx79d,"
//#define PL_SWAP_TABLE      "[FX@!%Nb"
//#define PL_ALLUPDATE_TABLE "kW3#ZQ!T"

//#define L_ADD_TABLE      "gh%j~w+x"
//#define L_INSERT_TABLE   "EjEZ)PX"
//#define L_EDIT_TABLE     "VngH68T:"
//#define L_DELETE_TABLE   "[u}iS63'"
//#define L_SWAP_TABLE     "1yJ2VDbs"

////undo redo only check operation id value. It is no means.
//#define E_ONLY_SCHEDULER    "krMQk[Me"
//#define S_ONLY_SCHEDULER    "YW)5K?g9"
//#define PL_ONLY_SCHEDULER   "d:&Dp<HB"
//#define PR_ONLY_SCHEDULER   "y0Dl)ZXR"

class PROCESSXMLLISTGENERATORSHARED_EXPORT ProcessXmlListGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ProcessXmlListGenerator(QObject *parent = nullptr);
    ~ProcessXmlListGenerator();

    //tab and list connection map
    enum{NORMAL, SEARCH, PLUGINS, OTHER};

    //inner table operation
    enum{TABLE_ADD,TABLE_EDIT,TABLE_INSERT,TABLE_DELETE
         ,TABLE_CUT,TABLE_PASTE,TABLE_DRAGDROP,TABLE_UP,TABLE_DOWN,TABLE_SWAP
         ,TABLE_ALLUPDATE};

    //connect ui to xml operation of tree
    enum{TREE_ADD,TREE_EDIT,TREE_INSERT,TREE_DELETE,TREE_UP,TREE_DOWN,TREE_SWAP};

    //change combined structure to separated structure
    void createSeparateList(QList<QStringList> *ctos);

    //get structure firstpos info
    void getListStructure(QList<QStringList> *ctos, QHash<int, int> *posinfo);

    //check info
    int getType(QString type);
    int fetchCmdFirstPos(QString tag, const QList<QStringList> *loadbase);

    //take specific item string
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos);
    QString fetch(QString tag, QString value, QString attr,const QList<QStringList> *loadbase);

private:

    //xml attributes (temp)
    const QString ALL_TYPE                   = "type";

    // info
    const QString I_NAME                     = "iname";
    const QString I_VERSION                  = "ver";
    const QString I_AUTHOR                   = "author";
    const QString I_DESCRIPTION              = "desc";
    const QString I_FILEINPUT                = "finput";
    const QString I_FILEINPUT_SEARCHCHECK    = "sinput";
    const QString I_FILESEARCH_NAME          = "fsname";
    const QString I_RECURSIVE_LOOP           = "rloop";
    const QString I_RECURSIVE_LOOPARGCOUNT   = "rlarg";
    const QString I_RECURSIVE_LOOPCOUNT      = "reloop";

    // search
    const QString S_NAME                     = "sname";
    const QString S_SEPARATOR                = "sep";
    const QString S_VARIANT                  = "var";
    const QString S_OUTPUTFILE               = "output";
    const QString S_OUTPUTFILETYPE           = "fitype";

    // profile

    // XML tag "prname";
    const QString PR_FILEPATH                = "prfile";

    // exec(normal)
    const QString E_TIMEOUT                  = "timeout";
    const QString E_RUNDETACH                = "detach";
    const QString E_CMD                      = "exc";
    const QString E_CMDARGCOUNT              = "cmdc";

    // plugin(extrafunc)
    const QString PL_NAME                    = "plname";
    const QString PL_FILEPATH                = "plfile";
    const QString PL_CMD                     = "plc";
    const QString PL_CMDARGCOUNT             = "pcmdc";

    // temp
    const QString TE_STACKEDWIDGET_POSITION  = "istack";

    // local
    const QString L_VARIANT                  = "lvar";
    const QString L_VAR_COUNT                = "localc";

    // value define (index 1)
    const QString TYPE_INFO                  = "info";
    const QString TYPE_EXEC                  = "normal";
    const QString TYPE_SEARCH                = "search";
    const QString TYPE_SCRIPT                = "script";
    const QString TYPE_ANOTHER               = "other";

    const QString TYPE_ALLINCLUDE            = "temp";
    const QString TYPE_LOCAL                 = "local";

    // attribute define (index 2)
    // all
    const QString ATTR_ONLY_SCHEDULER        = "only";
    const QString ATTR_POSNUM                = "id";
    const QString ATTR_RADIOBUTTONPOS        = "radio";
    const QString ATTR_MAXCOUNT              = "max";
    const QString ATTR_NONE                  = "";

    // normal
    const QString ATTR_TIMEOUTMS             = "dur";

    // local
    const QString ATTR_LOCALVALUE            = "lval";
};

#endif // PROCESSXMLLISTGENERATOR_H
