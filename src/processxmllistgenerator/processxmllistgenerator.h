#ifndef PROCESSXMLLISTGENERATOR_H
#define PROCESSXMLLISTGENERATOR_H

#include "processxmllistgenerator_global.h"
#include <QObject>
#include <QHash>
#include <QDateTime>
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
 * QStringList structure createNormalList
 * 0    :only_bool   :
 * 1    :timeout_bool:
 * 2    :timeout_ms  :
 * 3    :cmdc_counts :
 * 4    :cmd_text    :
 * -          *
 * -          *
 * -          *
 * - it follows to the end.
 *
 * QStringList structure createSearchList
 * 0    :only_bool   :
 * 1    :name_text   :
 * 2    :name_id_data:
 * 3    :sep_text    :
 * 4    :variant_text:
 * 5    :output_text :
 * 6    :radio_data  :
 *
 * QStringList structure createScriptList
 * 0    :only_bool  :
 * 1    :name_text  :
 * 2    :var_text   :
 * 3    :cmdc_counts:
 * 4    :cmd_text   :
 * -          *
 * -          *
 * -          *
 * - it follows to the end.
 *
 * QStringList structure createOtherList
 * 0    :only_bool   :
 * 1    :name_text   :
 * 2    :file_text   :
 *
 */


// tag define (index 0)
// all
static const QString ALL_TYPE =                   "type";
static const QString ALL_CMDVALUE =               "cmd";

// info
static const QString I_NAME =                     "iname";
static const QString I_VERSION =                  "ver";
static const QString I_AUTHOR =                   "author";
static const QString I_DESCRIPTION =              "desc";
static const QString I_FILEINPUT =                "finput";
static const QString I_FILEINPUT_SEARCHCHECK =    "sinput";
static const QString I_FILESEARCH_NAME =          "fsname";
static const QString I_RECURSIVE_LOOP =           "rloop";
static const QString I_RECURSIVE_LOOPARGCOUNT =   "rlarg";
static const QString I_RECURSIVE_LOOPCOUNT =      "reloop";

// search
static const QString S_NAME =                     "sname";
static const QString S_SEPARATOR =                "sep";
static const QString S_VARIANT =                  "var";
static const QString S_OUTPUTFILE =               "output";

// profile(other)
static const QString PR_NAME =                    "prname";
static const QString PR_FILEPATH =                "prfile";

// exec(normal)
static const QString E_TIMEOUT =                  "timeout";
static const QString E_CMDARGCOUNT =              "cmdc";

// plugin(extrafunc)
static const QString PL_NAME =                    "plname";
static const QString PL_FILEPATH =                "plfile";
static const QString PL_CMDARGCOUNT =             "pcmdc";

// temp
static const QString TE_STACKEDWIDGET_POSITION =  "istack";

// local
static const QString L_VARIANT =                  "lvar";
static const QString L_VAR_COUNT =                "localc";

// value define (index 1)
static const QString TYPE_INFO =                  "info";
static const QString TYPE_EXEC =                  "normal";
static const QString TYPE_SEARCH =                "search";
static const QString TYPE_SCRIPT =                "script";
static const QString TYPE_ANOTHER =               "other";

static const QString TYPE_ALLINCLUDE =            "temp";
static const QString TYPE_LOCAL =                 "local";

// attribute define (index 2)
// all
static const QString ATTR_ONLY_SCHEDULER =        "only";
static const QString ATTR_POSNUM =                "id";
static const QString ATTR_RADIOBUTTONPOS =        "radio";
static const QString ATTR_MAXCOUNT =              "max";
static const QString ATTR_NONE =                  "";

// normal
static const QString ATTR_TIMEOUTMS =             "dur";

// local
static const QString ATTR_LOCALVALUE =            "lval";

// undo redo operation command
static const QString UNDOREDO_ADD =               "ADD";
static const QString UNDOREDO_DELETE =            "DEL";
static const QString UNDOREDO_INSERT =            "INS";


class PROCESSXMLLISTGENERATORSHARED_EXPORT ProcessXmlListGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ProcessXmlListGenerator(QObject *parent = nullptr);
    ~ProcessXmlListGenerator();

    //tab and list connection map
    enum{NORMAL, SEARCH, EXTRAFUNC, OTHER};

    //inner list connection map // "_" means "and"
    //used in Undo Redo Command
//    enum{INFO_NAME,INFO_VER,INFO_AUTHOR,INFO_DESCRIPT,INFO_FINPUT,
//         INFO_SINPUT,INFO_RLOOP,INFO_RLARG,INFO_RELOOP,
//         LOCALVARIANT,CURRENTONLY,INNERSTACK,
//         SEARCHONLY,SEARCHNAME_ID,SEPARATOR,VARIANT,OUTPUTFILE,OUTPUT_RADIO,
//         OTHERONLY,OTHERNAME,OTHERFILEPATH,
//         NORMALONLY,TIMEOUT_DURITION,NCMDCOUNT,
//         EXTRAFUNCONLY,EXTRANAME,EXTRAFILE,ECMDCOUNT};

    //inner table operation
    enum{TABLE_ADD,TABLE_EDIT,TABLE_INSERT,TABLE_DELETE,TABLE_UP,TABLE_DOWN,TABLE_SWAP};

    //list add structure
    void createNewList(QList<QStringList> *newlist);

    //list create function
//    void createInfoList(QList<QStringList> *newlist, QStringList *list);
    void createLocalList(QList<QStringList> *newlist, QStringList *list);

    //combined above list
//    void createCombineList(QList<QStringList> *newlist, int index, QHash<int, QStringList *> *combine);

    //change combined structure to separated structure
    void createSeparateList(QList<QStringList> *ctos);

    //get structure firstpos info
    void getListStructure(QList<QStringList> *ctos, QHash<int, int> *posinfo);

    //check info
    int getType(QString type);

    //for change inner table structures
    static QStringList createCmdElement(QString value, int index);
    int fetchCmdFirstPos(QString tag, int firstpos, const QList<QStringList> *loadbase);

    //take specific item string
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos);

    //replace xml element
    void replaceElementList(QString tag, QString attr, int targetindex, QString replacestr, QList<QList<QStringList> *> *cache);
    void replaceElementList(int tableindex, int targetindex, QString replacestr, int skip, QList<QList<QStringList> *> *cache);

//private:
    //separated list create function
//    void createNormalList(QList<QStringList> *newlist, QStringList *list);
//    void createSearchList(QList<QStringList> *newlist, QStringList *list);
//    void createExtraFuncList(QList<QStringList> *newlist, QStringList *list);
//    void createOtherList(QList<QStringList> *newlist, QStringList *list);

    int getId(QString tag);

private:
    //keystore for undo or redo
    QHash<QString, int> generateId;
};

#endif // PROCESSXMLLISTGENERATOR_H
