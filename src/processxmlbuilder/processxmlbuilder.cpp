#include "processxmlbuilder.h"

ProcessXmlBuilder::ProcessXmlBuilder(QObject *)
{

}

ProcessXmlBuilder::~ProcessXmlBuilder()
{

}

void ProcessXmlBuilder::setLoadPath(QString filepath)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::setLoadPath] : " << filepath;
#endif

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
    qDebug() << "[ProcessXmlBuilder::readItem] :" << itemid;
#endif
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool ProcessXmlBuilder::readAllItem(QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::readAllItem]";
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
    newlist.append((QStringList() << ALL_TYPE << TYPE_INFO));
    newlist.append((QStringList() << I_NAME << ""));
    newlist.append((QStringList() << I_VERSION << ""));
    newlist.append((QStringList() << I_AUTHOR << ""));
    newlist.append((QStringList() << I_DESCRIPTION << ""));
//    newlist.append((QStringList() << FILEINPUT << "yes" << FILEINPUT_SEARCHCHECK << "no"));
    newlist.append((QStringList() << I_FILEINPUT << "yes"));
    newlist.append((QStringList() << I_FILEINPUT_SEARCHCHECK << "no"));
    newlist.append((QStringList() << I_FILESEARCH_NAME << "" << ATTR_POSNUM << ""));
    newlist.append((QStringList() << I_RECURSIVE_LOOP << "yes" << ATTR_MAXCOUNT << "1"));
    newlist.append((QStringList() << I_RECURSIVE_LOOPARGCOUNT << "1"));
    newlist.append((QStringList() << I_RECURSIVE_LOOPCOUNT << "0"));

    addItem(&newlist);
    newlist.clear();

    //local element
    newlist.append((QStringList() << ALL_TYPE << TYPE_LOCAL));
    newlist.append((QStringList() << L_VAR_COUNT << "0"));

    addItem(&newlist);
    newlist.clear();
}

int ProcessXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void ProcessXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
//    if(element.contains(QRegularExpression("^(name|ver|var|author|desc|finput|sinput|rlarg|reloop|cmdc|localc|sep|file|istack)$")))
//    {
//        //add element and text
//        list->append(QStringList() << element << rxml->readElementText());
//    }

    if(element == I_NAME
            || element == I_VERSION
            || element == S_VARIANT
            || element == I_AUTHOR
            || element == I_DESCRIPTION
            || element == I_FILEINPUT
            || element == I_FILEINPUT_SEARCHCHECK
            || element == I_RECURSIVE_LOOPARGCOUNT
            || element == I_RECURSIVE_LOOPCOUNT
            || element == E_CMDARGCOUNT
            || element == E_RUNDETACH
            || element == L_VAR_COUNT
            || element == S_SEPARATOR
            || element == S_OUTPUTFILETYPE
            || element == PR_FILEPATH
            || element == PL_CMDARGCOUNT
            || element == PL_FILEPATH
            || element == TE_STACKEDWIDGET_POSITION)
    {
        //add element and text
        list->append(QStringList() << element << rxml->readElementText());
    }

    if(element == ALL_TYPE)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_ONLY_SCHEDULER << rxml->attributes().value(ATTR_ONLY_SCHEDULER).toString());
    }

    if(element == E_TIMEOUT)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_TIMEOUTMS << rxml->attributes().value(ATTR_TIMEOUTMS).toString());
    }

    if(element == I_RECURSIVE_LOOP)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_MAXCOUNT << rxml->attributes().value(ATTR_MAXCOUNT).toString());
    }

    if(/*element == ALL_CMDVALUE*/
            element == E_CMD
            || element == PL_CMD
            || element == S_NAME
            || element == I_FILESEARCH_NAME)
    {
        //add to QList
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_POSNUM << rxml->attributes().value(ATTR_POSNUM).toString());
    }

    if(element == L_VARIANT)
    {
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_LOCALVALUE << rxml->attributes().value(ATTR_LOCALVALUE).toString());
    }

    if(element == S_OUTPUTFILE)
    {
        list->append(QStringList() << element << rxml->readElementText()
                     << ATTR_RADIOBUTTONPOS << rxml->attributes().value(ATTR_RADIOBUTTONPOS).toString());
    }

}
