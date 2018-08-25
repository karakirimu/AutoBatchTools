#include "processxmlbuilder.h"

ProcessXmlBuilder::ProcessXmlBuilder(QObject *)
{

}

ProcessXmlBuilder::~ProcessXmlBuilder()
{

}

void ProcessXmlBuilder::setLoadPath(QString filepath)
{
    qDebug() << "ProcessXmlBuilder: setLoadPath: " << filepath;
    if(filepath != ""){
        //set new file
        setFileName(filepath);

        if(count() == 0) createDocument();

    }else{
        //re create file object
        if(file->isOpen()) file->close();

        delete file;
        file = new QFile();

    }
}

void ProcessXmlBuilder::setLoadBlankPath(QString filepath)
{
    if(filepath != ""){
        //set new file
        setFileName(filepath);

        if(count() == 0){
            createXmlBaseDocument(ROOTELEMENT);
        }

    }
}

//void ProcessXmlBuilder::delLoadPath()
//{
//    if(file->exists()){
//        file->close();
//        if(!file->remove()){
//            qDebug() << file->errorString();
//        }
//    }
//}

bool ProcessXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "ProcessXmlBuilder: readItem :" << itemid;
#endif
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool ProcessXmlBuilder::readAllItem(QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "ProcessXmlBuilder: readAllItem :";
#endif
    return Xmlbuilder::readAllItem(FIRSTLAYER, ATTR, itemlist);
}

bool ProcessXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool ProcessXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool ProcessXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void ProcessXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

void ProcessXmlBuilder::copyItem(int itemid)
{
    QList<QStringList> tmp;
    readItem(itemid, &tmp);

    //add new item
    addItem(&tmp);
}

bool ProcessXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void ProcessXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
    QList<QStringList> newlist;

    //info element
    newlist.append((QStringList() << TYPE << TYPE_INFO));
    newlist.append((QStringList() << NAME_INFO << ""));
    newlist.append((QStringList() << VERSION << ""));
    newlist.append((QStringList() << AUTHOR << ""));
    newlist.append((QStringList() << DESCRIPTION << ""));
//    newlist.append((QStringList() << FILEINPUT << "yes" << FILEINPUT_SEARCHCHECK << "no"));
    newlist.append((QStringList() << FILEINPUT << "yes"));
    newlist.append((QStringList() << FILEINPUT_SEARCHCHECK << "no"));
    newlist.append((QStringList() << FILESEARCH_NAME << "" << ATTR_POSNUM << ""));
    newlist.append((QStringList() << RECURSIVE_LOOP << "yes" << ATTR_MAXCOUNT << "1"));
    newlist.append((QStringList() << RECURSIVE_LOOPARGCOUNT << "1"));
    newlist.append((QStringList() << RECURSIVE_LOOPCOUNT << "0"));

    addItem(&newlist);
    newlist.clear();

    //local element
    newlist.append((QStringList() << TYPE << TYPE_LOCAL));
    newlist.append((QStringList() << LOCAL_VAR_COUNT << "0"));

    addItem(&newlist);
    newlist.clear();
}

int ProcessXmlBuilder::count()
{
    return getSpecifiedElementItemsCount(FIRSTLAYER);
}

//If there is no element of "attr", assign PROCESS_NONE to "attr"
QString ProcessXmlBuilder::fetch(QString tag, QString attr, const QList<QStringList> *loadbase)
{
    int count = loadbase->count();
    int i = 0;
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

void ProcessXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
//    if(element.contains(QRegularExpression("^(name|ver|var|author|desc|finput|sinput|rlarg|reloop|cmdc|localc|sep|file|istack)$")))
//    {
//        //add element and text
//        list->append(QStringList() << element << rxml->readElementText());
//    }

    if(element == NAME_INFO
            || element == VERSION
            || element == SEARCH_VARIANT
            || element == AUTHOR
            || element == DESCRIPTION
            || element == FILEINPUT
            || element == FILEINPUT_SEARCHCHECK
            || element == RECURSIVE_LOOPARGCOUNT
            || element == RECURSIVE_LOOPCOUNT
            || element == EXEC_CMDARGCOUNT
            || element == LOCAL_VAR_COUNT
            || element == SEARCH_SEPARATOR
            || element == PROFILE_FILEPATH
            || element == STACKEDWIDGET_POSITION)
    {
        //add element and text
        list->append(QStringList() << element << rxml->readElementText());
    }

    if(element == TYPE)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_ONLY_SCHEDULER << rxml->attributes().value(ATTR_ONLY_SCHEDULER).toString());
    }

    if(element == EXEC_TIMEOUT)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_TIMEOUTMS << rxml->attributes().value(ATTR_TIMEOUTMS).toString());
    }

    if(element == RECURSIVE_LOOP)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_MAXCOUNT << rxml->attributes().value(ATTR_MAXCOUNT).toString());
    }

    if(element == CMDVALUE
            || element == NAME_SEARCH
            || element == FILESEARCH_NAME)
    {
        //add to QList
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_POSNUM << rxml->attributes().value(ATTR_POSNUM).toString());
    }

    if(element == LOCAL_VARIANT)
    {
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_LOCALVALUE << rxml->attributes().value(ATTR_LOCALVALUE).toString());
    }

    if(element == SEARCH_OUTPUTFILE)
    {
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_RADIOBUTTONPOS << rxml->attributes().value(ATTR_RADIOBUTTONPOS).toString());
    }

}
