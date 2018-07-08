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
    newlist.append((QStringList() << "type" << "info"));
    newlist.append((QStringList() << "name" << ""));
    newlist.append((QStringList() << "ver" << ""));
    newlist.append((QStringList() << "author" << ""));
    newlist.append((QStringList() << "desc" << ""));
    newlist.append((QStringList() << "finput" << "yes" << "sinput" << "no"));
    newlist.append((QStringList() << "sinput" << "no"));
    newlist.append((QStringList() << "fsname" << "" << "id" << ""));
    newlist.append((QStringList() << "rloop" << "yes" << "max" << "1"));
    newlist.append((QStringList() << "rlarg" << "1"));
    newlist.append((QStringList() << "reloop" << "0"));

    addItem(&newlist);
    newlist.clear();

    //local element
    newlist.append((QStringList() << "type" << "local"));
    newlist.append((QStringList() << "localc" << "0"));

    addItem(&newlist);
    newlist.clear();
}

int ProcessXmlBuilder::count()
{
    return getSpecifiedElementItemsCount(FIRSTLAYER);
}

//int ProcessXmlBuilder::firstPosTempNormal() const
//{
//    return 10;
//}

//int ProcessXmlBuilder::firstPosTempSearch() const
//{
//    return 2;
//}

//int ProcessXmlBuilder::firstPosTempScript() const
//{
//    return 13;
//}

//int ProcessXmlBuilder::firstPosTempOther() const
//{
//    return 7;
//}

void ProcessXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
//    if(element == "name"
//            || element == "ver"
//            || element == "var"
//            || element == "author"
//            || element == "desc"
//            || element == "uptime"
////            || element == "nowait"
//            || element == "cmdc"
//            || element == "localc"
//            || element == "sep"
//            || element == "file"
//            /*|| element == "fstack"*/
//            || element == "istack")
    if(element.contains(QRegularExpression("^(name|ver|var|author|desc|finput|sinput|rlarg|reloop|cmdc|localc|sep|file|istack)$")))
    {
        //add element and text
        list->append(QStringList() << element << rxml->readElementText());
    }

    if(element == "type")
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << "only" << rxml->attributes().value("only").toString());
    }

    if(element == "timeout")
    {
//        QString during = rxml->attributes().value("dur").toString();
//        QStringList data;
//        //add element and text
//        data.append(element);
//        data.append(rxml->readElementText());
//        //add attributes and data
//        data.append("dur");
//        data.append(during);

        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << "dur" << rxml->attributes().value("dur").toString());
    }

    if(element == "rloop")
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << "max" << rxml->attributes().value("max").toString());
    }

    if(element == "cmd" || element == "sname" || element == "fsname")
    {
//        QString id = rxml->attributes().value("id").toString();
//        QStringList data;
//        //add element and text
//        data.append(element);
//        data.append(rxml->readElementText());
//        //add attributes and data
//        data.append("id");
//        data.append(id);

        //add to QList
        list->append(QStringList() << element << rxml->readElementText()
                     << "id" << rxml->attributes().value("id").toString());
    }

    if(element == "lvar")
    {
//        QString lval = rxml->attributes().value("lval").toString();
//        QStringList data;
//        //add element and text
//        data.append(element);
//        data.append(rxml->readElementText());
//        //add attributes and data
//        data.append("lval");
//        data.append(lval);

//        //add to QList
//        list->append(data);

        list->append(QStringList() << element << rxml->readElementText()
                     << "lval" << rxml->attributes().value("lval").toString());
    }

    if(element == "output")
    {
//        QString radio = rxml->attributes().value("radio").toString();
//        QStringList data;
//        //add element and text
//        data.append(element);
//        data.append(rxml->readElementText());
//        //add attributes and data
//        data.append("radio");
//        data.append(radio);

//        //add to QList
//        list->append(data);

        list->append(QStringList() << element << rxml->readElementText()
                     << "radio" << rxml->attributes().value("radio").toString());
    }

}
