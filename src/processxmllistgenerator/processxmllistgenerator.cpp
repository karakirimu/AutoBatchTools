#include "processxmllistgenerator.h"

ProcessXmlListGenerator::ProcessXmlListGenerator(QObject *parent)
    : QObject (parent)
{
    // tag define (index 0)
    // all
//    const QString ALL_TYPE =                   "type";
//    const QString ALL_CMDVALUE =               "cmd";

    generateId.insert(ALL_TYPE, 0);
    generateId.insert(ALL_CMDVALUE, 1);

    // info
//    const QString I_NAME =                     "iname";
//    const QString I_VERSION =                  "ver";
//    const QString I_AUTHOR =                   "author";
//    const QString I_DESCRIPTION =              "desc";
//    const QString I_FILEINPUT =                "finput";
//    const QString I_FILEINPUT_SEARCHCHECK =    "sinput";
//    const QString I_FILESEARCH_NAME =          "fsname";
//    const QString I_RECURSIVE_LOOP =           "rloop";
//    const QString I_RECURSIVE_LOOPARGCOUNT =   "rlarg";
//    const QString I_RECURSIVE_LOOPCOUNT =      "reloop";

    generateId.insert(I_NAME, 100);
    generateId.insert(I_VERSION, 101);
    generateId.insert(I_AUTHOR, 102);
    generateId.insert(I_DESCRIPTION, 103);
    generateId.insert(I_FILEINPUT, 104);
    generateId.insert(I_FILEINPUT_SEARCHCHECK, 105);
    generateId.insert(I_FILESEARCH_NAME, 106);
    generateId.insert(I_RECURSIVE_LOOP, 107);
    generateId.insert(I_RECURSIVE_LOOPARGCOUNT, 108);
    generateId.insert(I_RECURSIVE_LOOPCOUNT, 109);

    // search
//    const QString S_NAME =                     "sname";
//    const QString S_SEPARATOR =                "sep";
//    const QString S_VARIANT =                  "var";
//    const QString S_OUTPUTFILE =               "output";

    generateId.insert(S_NAME, 200);
    generateId.insert(S_SEPARATOR, 201);
    generateId.insert(S_VARIANT, 202);
    generateId.insert(S_OUTPUTFILE, 203);

    // profile(other)
//    const QString PR_NAME =                    "prname";
//    const QString PR_FILEPATH =                "prfile";

    generateId.insert(PR_NAME, 300);
    generateId.insert(PR_FILEPATH, 301);

    // exec(normal)
//    const QString E_TIMEOUT =                  "timeout";
//    const QString E_CMDARGCOUNT =              "cmdc";

    generateId.insert(E_TIMEOUT, 400);
    generateId.insert(E_CMDARGCOUNT, 401);

    // plugin(extrafunc)
//    const QString PL_NAME =                    "plname";
//    const QString PL_FILEPATH =                "plfile";
//    const QString PL_CMDARGCOUNT =             "pcmdc";

    generateId.insert(PL_NAME, 500);
    generateId.insert(PL_FILEPATH, 501);
    generateId.insert(PL_CMDARGCOUNT, 502);

    // temp
//    const QString TE_STACKEDWIDGET_POSITION =  "istack";

    generateId.insert(TE_STACKEDWIDGET_POSITION, 600);

    // local
//    const QString L_VARIANT =                  "lvar";
//    const QString L_VAR_COUNT =                "localc";

    generateId.insert(L_VARIANT, 700);
    generateId.insert(L_VAR_COUNT, 701);

    // value define (index 1)
//    const QString TYPE_INFO =                  "info";
//    const QString TYPE_EXEC =                  "normal";
//    const QString TYPE_SEARCH =                "search";
//    const QString TYPE_SCRIPT =                "script";
//    const QString TYPE_ANOTHER =               "other";

//    const QString TYPE_ALLINCLUDE =            "temp";
//    const QString TYPE_LOCAL =                 "local";

    generateId.insert(TYPE_INFO, 800);
    generateId.insert(TYPE_EXEC, 801);
    generateId.insert(TYPE_SEARCH, 802);
    generateId.insert(TYPE_SCRIPT, 803);
    generateId.insert(TYPE_ANOTHER, 804);

    generateId.insert(TYPE_ALLINCLUDE, 805);
    generateId.insert(TYPE_LOCAL, 806);

    // attribute define (index 2)
    // all
//    const QString ATTR_ONLY_SCHEDULER =        "only";
//    const QString ATTR_POSNUM =                "id";
//    const QString ATTR_RADIOBUTTONPOS =        "radio";
//    const QString ATTR_MAXCOUNT =              "max";
//    const QString ATTR_NONE =                  "";

    generateId.insert(ATTR_ONLY_SCHEDULER, 900);
    generateId.insert(ATTR_POSNUM, 901);
    generateId.insert(ATTR_RADIOBUTTONPOS, 902);
    generateId.insert(ATTR_MAXCOUNT, 903);
    generateId.insert(ATTR_NONE, 904);

    // normal
//    const QString ATTR_TIMEOUTMS =             "dur";

    generateId.insert(ATTR_TIMEOUTMS, 905);

    // local
//    const QString ATTR_LOCALVALUE =            "lval";

    generateId.insert(ATTR_LOCALVALUE, 906);
}

ProcessXmlListGenerator::~ProcessXmlListGenerator()
{

}

///DEPENDS_XML
void ProcessXmlListGenerator::createNewList(QList<QStringList> *newlist)
{
    //initial data
    newlist->append((QStringList() << ALL_TYPE << TYPE_ALLINCLUDE << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << TE_STACKEDWIDGET_POSITION << "0"));
    //search
    newlist->append((QStringList() << ALL_TYPE << TYPE_SEARCH << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << S_NAME << "" << ATTR_POSNUM << "-1"));
    newlist->append((QStringList() << S_SEPARATOR << ""));
    newlist->append((QStringList() << S_VARIANT << ""));
    newlist->append((QStringList() << S_OUTPUTFILE << "" << ATTR_RADIOBUTTONPOS << "0"));
    //other
    newlist->append((QStringList() << ALL_TYPE << TYPE_ANOTHER << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << PR_NAME << ""));
    newlist->append((QStringList() << PR_FILEPATH << ""));
    //normal
    newlist->append((QStringList() << ALL_TYPE << TYPE_EXEC << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << E_TIMEOUT << "no" << ATTR_TIMEOUTMS << "30000"));
    newlist->append((QStringList() << E_CMDARGCOUNT << "0"));
    //extrafunc
    newlist->append((QStringList() << ALL_TYPE << TYPE_SCRIPT << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << PL_NAME << ""));
    newlist->append((QStringList() << PL_FILEPATH << ""));
    newlist->append((QStringList() << PL_CMDARGCOUNT << "0"));

}

///DEPENDS_XML
//void ProcessXmlListGenerator::createInfoList(QList<QStringList> *newlist, QStringList *list)
//{
//    newlist->append((QStringList() << TYPE << TYPE_INFO));
//    newlist->append((QStringList() << "name" << list->at(0)));
//    newlist->append((QStringList() << VERSION << list->at(1)));
//    newlist->append((QStringList() << AUTHOR << list->at(2)));
//    newlist->append((QStringList() << DESCRIPTION << list->at(3)));
//    newlist->append((QStringList() << FILEINPUT << list->at(4)));
//    newlist->append((QStringList() << FILEINPUT_SEARCHCHECK << list->at(5)));
//    newlist->append((QStringList() << RECURSIVE_LOOP << list->at(6) << ATTR_POSNUM << list->at(7)));
//    newlist->append((QStringList() << RECURSIVE_LOOP << list->at(8) << ATTR_MAXCOUNT << list->at(9)));
//    newlist->append((QStringList() << RECURSIVE_LOOPARGCOUNT << list->at(10)));
//    newlist->append((QStringList() << RECURSIVE_LOOPCOUNT << list->at(11)));

//}

///DEPENDS_XML
void ProcessXmlListGenerator::createLocalList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = static_cast<QString>(list->at(0)).toInt() * 2;
    newlist->append((QStringList() << ALL_TYPE << TYPE_LOCAL));
    newlist->append((QStringList() << L_VAR_COUNT << list->at(0)));
    for(int i = 0; i < rcount; i+=2){
        newlist->append((QStringList() << L_VARIANT
                         << list->at(i + 1) << ATTR_LOCALVALUE << list->at(i + 2)));
    }
}

/////DEPENDS_XML
//void ProcessXmlListGenerator::createNormalList(QList<QStringList> *newlist, QStringList *list)
//{
//    int rcount = static_cast<QString>(list->at(3)).toInt();
//    newlist->append((QStringList() << TYPE << TYPE_EXEC << ATTR_ONLY_SCHEDULER << list->at(0)));
//    newlist->append((QStringList() << EXEC_TIMEOUT << list->at(1) << ATTR_TIMEOUTMS << list->at(2)));
//    newlist->append((QStringList() << EXEC_CMDARGCOUNT << list->at(3)));
//    for(int i = 0; i < rcount; i++){
//        newlist->append((QStringList() << CMDVALUE
//                         << list->at(i + 4) << ATTR_POSNUM << QString::number(i)));
//    }
//}

/////DEPENDS_XML
//void ProcessXmlListGenerator::createSearchList(QList<QStringList> *newlist, QStringList *list)
//{
//    newlist->append((QStringList() << TYPE << TYPE_SEARCH << ATTR_ONLY_SCHEDULER << list->at(0)));
//    newlist->append((QStringList() << NAME_SEARCH << list->at(1) << ATTR_POSNUM << list->at(2)));
//    newlist->append((QStringList() << SEARCH_SEPARATOR << list->at(3)));
//    newlist->append((QStringList() << SEARCH_VARIANT << list->at(4)));
//    newlist->append((QStringList() << SEARCH_OUTPUTFILE << list->at(5) << ATTR_RADIOBUTTONPOS << list->at(6)));
//}

/////DEPENDS_XML
//void ProcessXmlListGenerator::createExtraFuncList(QList<QStringList> *newlist, QStringList *list)
//{
//    int rcount = static_cast<QString>(list->at(3)).toInt();
//    newlist->append((QStringList() << TYPE << TYPE_SCRIPT << ATTR_ONLY_SCHEDULER << list->at(0)));
//    newlist->append((QStringList() << "name" << list->at(1)));
//    newlist->append((QStringList() << PLUGIN_FILEPATH << list->at(2)));
//    newlist->append((QStringList() << EXEC_CMDARGCOUNT << list->at(3)));
//    for(int i = 0; i < rcount; i++){
//        newlist->append((QStringList() << CMDVALUE
//                         << list->at(i + 4) << ATTR_POSNUM << QString::number(i)));
//    }
//}

/////DEPENDS_XML
//void ProcessXmlListGenerator::createOtherList(QList<QStringList> *newlist, QStringList *list)
//{
//    newlist->append((QStringList() << TYPE << TYPE_ANOTHER << ATTR_ONLY_SCHEDULER << list->at(0)));
//    newlist->append((QStringList() << "name" << list->at(1)));
//    newlist->append((QStringList() << PROFILE_FILEPATH << list->at(2)));
//}

///DEPENDS_XML
//void ProcessXmlListGenerator::createCombineList(QList<QStringList> *newlist, int index, QHash<int, QStringList *> *combine)
//{
//    // basic infomation
//    newlist->append((QStringList() << TYPE << TYPE_ALLINCLUDE << ATTR_ONLY_SCHEDULER << combine->value(index)->at(0)));
//    newlist->append((QStringList() << STACKEDWIDGET_POSITION << QString::number(index)));

//    // TYPE << TYPE_SEARCH;
//    createSearchList(newlist, combine->value(SEARCH));

//    // TYPE << TYPE_ANOTHER;
//    createOtherList(newlist, combine->value(OTHER));

//    // TYPE << TYPE_EXEC;
//    createNormalList(newlist, combine->value(NORMAL));

//    // TYPE << TYPE_SCRIPT;
//    createExtraFuncList(newlist, combine->value(EXTRAFUNC));
//}

///DEPENDS_XML
void ProcessXmlListGenerator::createSeparateList(QList<QStringList> *ctos)
{
    //get selected index
    int index = static_cast<QString>(ctos->at(1).at(1)).toInt();

    //remove till first header
    while(!ctos->empty()){
        if(getType(ctos->first().at(1)) == index){
            break;
        }else{
            ctos->removeFirst();
        }
    }

    int i = 1;
    bool rem = false;
    while(i < ctos->count()){
        if(getType(ctos->at(i).at(1)) == -1 && !rem){
            i++;
        }else{
            rem = true;
            ctos->removeAt(i);
        }
    }
}

void ProcessXmlListGenerator::getListStructure(QList<QStringList> *ctos, QHash<int, int> *posinfo)
{
    //dynamic first index generator
    //take each attributes count
    QMutableListIterator<QStringList> i(*ctos);

    int c = 0;
    int cur = -1;
    while(i.hasNext()){
        cur = getType(i.next().at(1));
        if(cur > -1){
            posinfo->insert(cur, c);
        }
        c++;
    }
}

///DEPENDS_XML
int ProcessXmlListGenerator::getType(QString type)
{
    if(type == TYPE_EXEC) return NORMAL;
    if(type == TYPE_SEARCH) return SEARCH;
    if(type == TYPE_SCRIPT) return EXTRAFUNC;
    if(type == TYPE_ANOTHER) return OTHER;
    return -1;
}

QStringList ProcessXmlListGenerator::createCmdElement(QString value, int index)
{
    return QStringList() << ALL_CMDVALUE << value << ATTR_POSNUM << QString::number(index);
}

int ProcessXmlListGenerator::fetchCmdFirstPos(QString tag, int firstpos, const QList<QStringList> *loadbase)
{
    int count = loadbase->count();
    int i = firstpos;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            return i;
        }
        i++;
    }

    //cannot find
    return -1;
}

// If there is no element of "attr", assign PROCESS_NONE to "attr"
QString ProcessXmlListGenerator::fetch(QString tag, QString attr, const QList<QStringList> *loadbase)
{
    return this->fetch(tag, attr, loadbase, 0);
}

QString ProcessXmlListGenerator::fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos)
{
    int count = loadbase->count();
    int i = firstpos;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            if(attr == ATTR_NONE) return loadbase->at(i).at(1);

            listnummax = loadbase->at(i).count();
            if(listnummax > 3 && attr == loadbase->at(i).at(2)) return loadbase->at(i).at(3);
            if(listnummax > 5 && attr == loadbase->at(i).at(4)) return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return ATTR_NONE;
}

void ProcessXmlListGenerator::replaceElementList(QString tag, QString attr, int targetindex \
                                                 , QString replacestr, QList<QList<QStringList> *> *cache)
{
    QList<QStringList> *loadbase = cache->at(targetindex);
    int count = loadbase->count();
    int i = 0;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            if(attr == ATTR_NONE) {
                QStringList alist = loadbase->at(i);
                alist.replace(1, replacestr);
                cache->at(targetindex)->replace(i, alist);
            }

            listnummax = loadbase->at(i).count();

            if(listnummax > 3 && attr == loadbase->at(i).at(2)){
                QStringList alist = loadbase->at(i);
                alist.replace(3, replacestr);
                cache->at(targetindex)->replace(i, alist);
            }

            if(listnummax > 5 && attr == loadbase->at(i).at(4)){
                QStringList alist = loadbase->at(i);
                alist.replace(5, replacestr);
                cache->at(targetindex)->replace(i, alist);
            }
        }
        i++;
    }

    return;
}

void ProcessXmlListGenerator::replaceElementList(int tableindex, int targetindex, QString replacestr, \
                                                      int skip, QList<QList<QStringList> *> *cache)
{
    QStringList alist = cache->at(targetindex)->at(tableindex + skip);
    alist.replace(1, replacestr);
    cache->at(targetindex)->replace(tableindex + skip, alist);
}

int ProcessXmlListGenerator::getId(QString tag)
{
    return generateId.value(tag);
}

/* Dynamically generate the Id number necessary to go back and forward.
 * If the corresponding tag does not exist, a new Id is generated,
 * and if it is already generated, its Id is returned.
*/
//int ProcessXmlListGenerator::getId(QString tag)
//{
//    if(!generateId.contains(tag)){
//        // generate Id
//        generateId.insert(tag, generateId.keys().count());
//    }

//    // return generated Id
//    return generateId.value(tag);
//}
