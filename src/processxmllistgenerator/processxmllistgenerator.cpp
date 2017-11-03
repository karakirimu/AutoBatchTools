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
    newlist->append((QStringList() << "type" << "temp" << "only" << "no"));
    newlist->append((QStringList() << "istack" << "0"));
    //search
    newlist->append((QStringList() << "type" << "search" << "only" << "no"));
    newlist->append((QStringList() << "sname" << "" << "id" << "-1"));
    newlist->append((QStringList() << "sep" << ""));
    newlist->append((QStringList() << "var" << ""));
    newlist->append((QStringList() << "output" << "" << "radio" << "0"));
    //other
    newlist->append((QStringList() << "type" << "other" << "only" << "no"));
    newlist->append((QStringList() << "name" << ""));
    newlist->append((QStringList() << "file" << ""));
    //normal
    newlist->append((QStringList() << "type" << "normal" << "only" << "no"));
    newlist->append((QStringList() << "timeout" << "no" << "dur" << "30000"));
    newlist->append((QStringList() << "cmdc" << "0"));
    //extrafunc
    newlist->append((QStringList() << "type" << "script" << "only" << "no"));
    newlist->append((QStringList() << "name" << ""));
    newlist->append((QStringList() << "file" << ""));
    newlist->append((QStringList() << "cmdc" << "0"));

}

///DEPENDS_XML
void ProcessXmlListGenerator::createInfoList(QList<QStringList> *newlist, QStringList *list)
{
    newlist->append((QStringList() << "type" << "info"));
    newlist->append((QStringList() << "name" << list->at(0)));
    newlist->append((QStringList() << "ver" << list->at(1)));
    newlist->append((QStringList() << "author" << list->at(2)));
    newlist->append((QStringList() << "desc" << list->at(3)));
    newlist->append((QStringList() << "uptime" << QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss")));

}

///DEPENDS_XML
void ProcessXmlListGenerator::createLocalList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = ((QString)list->at(0)).toInt() * 2;
    newlist->append((QStringList() << "type" << "local"));
    newlist->append((QStringList() << "localc" << list->at(0)));
    for(int i = 0; i < rcount; i+=2){
        newlist->append((QStringList() << "lvar"
                         << list->at(i + 1) << "lval" << list->at(i + 2)));
    }
}

///DEPENDS_XML
void ProcessXmlListGenerator::createNormalList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = ((QString)list->at(3)).toInt();
    newlist->append((QStringList() << "type" << "normal" << "only" << list->at(0)));
    newlist->append((QStringList() << "timeout" << list->at(1) << "dur" << list->at(2)));
    newlist->append((QStringList() << "cmdc" << list->at(3)));
    for(int i = 0; i < rcount; i++){
        newlist->append((QStringList() << "cmd"
                         << list->at(i + 4) << "id" << QString::number(i)));
    }
}

///DEPENDS_XML
void ProcessXmlListGenerator::createSearchList(QList<QStringList> *newlist, QStringList *list)
{
    newlist->append((QStringList() << "type" << "search" << "only" << list->at(0)));
    newlist->append((QStringList() << "sname" << list->at(1) << "id" << list->at(2)));
    newlist->append((QStringList() << "sep" << list->at(3)));
    newlist->append((QStringList() << "var" << list->at(4)));
    newlist->append((QStringList() << "output" << list->at(5) << "radio" << list->at(6)));
}

///DEPENDS_XML
void ProcessXmlListGenerator::createExtraFuncList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = ((QString)list->at(3)).toInt();
    newlist->append((QStringList() << "type" << "script" << "only" << list->at(0)));
    newlist->append((QStringList() << "name" << list->at(1)));
    newlist->append((QStringList() << "file" << list->at(2)));
    newlist->append((QStringList() << "cmdc" << list->at(3)));
    for(int i = 0; i < rcount; i++){
        newlist->append((QStringList() << "cmd"
                         << list->at(i + 4) << "id" << QString::number(i)));
    }
}

///DEPENDS_XML
void ProcessXmlListGenerator::createOtherList(QList<QStringList> *newlist, QStringList *list)
{
    newlist->append((QStringList() << "type" << "other" << "only" << list->at(0)));
    newlist->append((QStringList() << "name" << list->at(1)));
    newlist->append((QStringList() << "file" << list->at(2)));
}

///DEPENDS_XML
void ProcessXmlListGenerator::createCombineList(QList<QStringList> *newlist, int index, QHash<int, QStringList *> *combine)
{
    // basic infomation
    newlist->append((QStringList() << "type" << "temp" << "only" << combine->value(index)->at(0)));
    newlist->append((QStringList() << "istack" << QString::number(index)));

    // "type" << "search";
    createSearchList(newlist, combine->value(SEARCH));

    // "type" << "other";
    createOtherList(newlist, combine->value(OTHER));

    // "type" << "normal";
    createNormalList(newlist, combine->value(NORMAL));

    // "type" << "script";
    createExtraFuncList(newlist, combine->value(EXTRAFUNC));
}

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
    if(type == "normal") return NORMAL;
    if(type == "search") return SEARCH;
    if(type == "script") return EXTRAFUNC;
    if(type == "other") return OTHER;
    return -1;
}

QStringList ProcessXmlListGenerator::createCmdElement(QString value, int index)
{
    return QStringList() << "cmd" << value << "id" << QString::number(index);
}
