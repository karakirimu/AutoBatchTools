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
static const QString ALL_TYPE =                   "type";

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
static const QString S_OUTPUTFILETYPE =           "fitype";

// profile

// XML tag "prname"
//static const QString PR_NAME =                    "prname";
//! XML tag "prfile"
static const QString PR_FILEPATH =                "prfile";

// exec(normal)
static const QString E_TIMEOUT =                  "timeout";
static const QString E_CMD =                      "exc";
static const QString E_CMDARGCOUNT =              "cmdc";

// plugin(extrafunc)
static const QString PL_NAME =                    "plname";
static const QString PL_FILEPATH =                "plfile";
static const QString PL_CMD =                     "plc";
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
static const QString UNDOREDO_SWAP =              "SWA";
static const QString UNDOREDO_MOVE =              "MOV";

static const QString UNDOREDO_E_TABLEADD =        "ET_ADD";
static const QString UNDOREDO_E_TABLEEDIT =       "ET_EDIT";
static const QString UNDOREDO_E_TABLEDEL =        "ET_DEL";
static const QString UNDOREDO_E_TABLEINS =        "ET_INS";
static const QString UNDOREDO_E_TABLESWAP =       "ET_SWAP";
static const QString UNDOREDO_E_TABLEMOVE =       "ET_MOVE";

static const QString UNDOREDO_PL_TABLEADD =       "PLT_ADD";
static const QString UNDOREDO_PL_TABLEEDIT =      "PLT_EDIT";
static const QString UNDOREDO_PL_TABLEDEL =       "PLT_DEL";
static const QString UNDOREDO_PL_TABLEINS =       "PLT_INS";
static const QString UNDOREDO_PL_TABLESWAP =      "PLT_SWAP";
static const QString UNDOREDO_PL_TABLEMOVE =      "PLT_MOVE";

static const QString UNDOREDO_LV_ADD =            "LV_ADD";
static const QString UNDOREDO_LV_EDIT =           "LV_EDIT";
static const QString UNDOREDO_LV_DEL =            "LV_DEL";
static const QString UNDOREDO_LV_INS =            "LV_INS";
static const QString UNDOREDO_LV_SWAP =           "LV_SWAP";

static const QString UNDOREDO_GV_ADD =            "GV_ADD";
static const QString UNDOREDO_GV_EDIT =           "GV_EDIT";
static const QString UNDOREDO_GV_DEL =            "GV_DEL";
static const QString UNDOREDO_GV_INS =            "GV_INS";
static const QString UNDOREDO_GV_SWAP =           "GV_SWAP";

//undo redo table operation id value. It is no means.
//?
static const QString E_ADD_TABLE    = "{(I*9g-v";
static const QString E_INSERT_TABLE = "M$7>O/Qb";
static const QString E_EDIT_TABLE   = "d/%o'q9l";
static const QString E_DELETE_TABLE = "ht9!YQ(:";
static const QString E_SWAP_TABLE   = ",5Dm&y/R";

static const QString PL_ADD_TABLE    = ")r]&mwqn";
static const QString PL_INSERT_TABLE = "8NGM:Yv3";
static const QString PL_EDIT_TABLE   = "UOq:>~`c";
static const QString PL_DELETE_TABLE = "/=gx79d,";
static const QString PL_SWAP_TABLE   = "[FX@!%Nb";

static const QString L_ADD_TABLE    = "gh%j~w+x";
static const QString L_INSERT_TABLE = "EjE;Z)PX";
static const QString L_EDIT_TABLE   = "VngH68T:";
static const QString L_DELETE_TABLE = "[u}iS63'";
static const QString L_SWAP_TABLE   = "1yJ2VDbs";

//undo redo only check operation id value. It is no means.
static const QString E_ONLY_SCHEDULER  = "krMQk[Me";
static const QString S_ONLY_SCHEDULER  = "YW)5K?g9";
static const QString PL_ONLY_SCHEDULER = "d:&Dp<HB";
static const QString PR_ONLY_SCHEDULER = "y0Dl)ZXR";

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
         ,TABLE_CUT,TABLE_PASTE,TABLE_DRAGDROP,TABLE_UP,TABLE_DOWN,TABLE_SWAP};

    //connect ui to xml operation of tree
    enum{TREE_ADD,TREE_EDIT,TREE_INSERT,TREE_DELETE,TREE_UP,TREE_DOWN,TREE_SWAP};

    //list add structure
    void createNewList(QList<QStringList> *newlist);

    //list create function
//    void createLocalList(QList<QStringList> *newlist, QStringList *list);

    //change combined structure to separated structure
    void createSeparateList(QList<QStringList> *ctos);

    //get structure firstpos info
    void getListStructure(QList<QStringList> *ctos, QHash<int, int> *posinfo);

    //check info
    int getType(QString type);

    //for change inner table structures
    static QStringList createExecElement(QString value, int index);
    static QStringList createPluginElement(QString value, int index);
    static QStringList createVariantElement(QStringList variants);

    int fetchCmdFirstPos(QString tag, const QList<QStringList> *loadbase);

    //take specific item string
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos);
    QString fetch(QString tag, QString value, QString attr,const QList<QStringList> *loadbase);

    //replace xml element
    void replaceElementList(QString tag, QString attr, int targetindex, QString replacestr, QList<QList<QStringList> *> *cache);
    void replaceElementList(int tableindex, int targetindex, QString replacestr, int skip, QList<QList<QStringList> *> *cache);
    void replaceElementList(int tableindex, int targetindex, QStringList replace, int skip, QList<QList<QStringList> *> *cache);
    void replaceTypeElement(QString edittype, QString attr, int targetindex, QString replacestr, QList<QList<QStringList> *> *cache);

    int getId(QString tag);

private:
    //keystore for undo or redo
    QHash<QString, int> generateId;
};

#endif // PROCESSXMLLISTGENERATOR_H
