#include "processXmlListGenerator.h"

ProcessXmlListGenerator::ProcessXmlListGenerator(QObject *parent)
    : QObject(parent)
{

}

ProcessXmlListGenerator::~ProcessXmlListGenerator()
{

}

///DEPENDS_XML
void ProcessXmlListGenerator::createNewList(QList<QStringList> *newlist)
{
    //initial data
    newlist->append((QStringList() << TYPE << TYPE_ALLINCLUDE << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << STACKEDWIDGET_POSITION << "0"));
    //search
    newlist->append((QStringList() << TYPE << TYPE_SEARCH << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << NAME_SEARCH << "" << ATTR_POSNUM << "-1"));
    newlist->append((QStringList() << SEARCH_SEPARATOR << ""));
    newlist->append((QStringList() << SEARCH_VARIANT << ""));
    newlist->append((QStringList() << SEARCH_OUTPUTFILE << "" << ATTR_RADIOBUTTONPOS << "0"));
    //other
    newlist->append((QStringList() << TYPE << TYPE_ANOTHER << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << NAME_PROFILE << ""));
    newlist->append((QStringList() << PROFILE_FILEPATH << ""));
    //normal
    newlist->append((QStringList() << TYPE << TYPE_EXEC << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << EXEC_TIMEOUT << "no" << ATTR_TIMEOUTMS << "30000"));
    newlist->append((QStringList() << EXEC_CMDARGCOUNT << "0"));
    //extrafunc
    newlist->append((QStringList() << TYPE << TYPE_SCRIPT << ATTR_ONLY_SCHEDULER << "no"));
    newlist->append((QStringList() << NAME_PLUGIN << ""));
    newlist->append((QStringList() << PLUGIN_FILEPATH << ""));
    newlist->append((QStringList() << EXEC_CMDARGCOUNT << "0"));

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
    int rcount = ((QString)list->at(0)).toInt() * 2;
    newlist->append((QStringList() << TYPE << TYPE_LOCAL));
    newlist->append((QStringList() << LOCAL_VAR_COUNT << list->at(0)));
    for(int i = 0; i < rcount; i+=2){
        newlist->append((QStringList() << LOCAL_VARIANT
                         << list->at(i + 1) << ATTR_LOCALVALUE << list->at(i + 2)));
    }
}

/////DEPENDS_XML
//void ProcessXmlListGenerator::createNormalList(QList<QStringList> *newlist, QStringList *list)
//{
//    int rcount = ((QString)list->at(3)).toInt();
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
//    int rcount = ((QString)list->at(3)).toInt();
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
    int index = ((QString)ctos->at(1).at(1)).toInt();

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
    return QStringList() << CMDVALUE << value << ATTR_POSNUM << QString::number(index);
}
