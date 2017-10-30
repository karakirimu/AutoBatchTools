#include "processxmlbuilder.h"

ProcessXmlBuilder::ProcessXmlBuilder(QObject *)
{

}

ProcessXmlBuilder::~ProcessXmlBuilder()
{

}

void ProcessXmlBuilder::setLoadPath(QString filepath)
{
    if(filepath != ""){
        //set new file
        setFileName(filepath);

        if(count() == 0){
            createDocument();
        }

    }else{
        //re create file object
        if(file->isOpen()){
            file->close();
        }

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
    newlist.append((QStringList() << "uptime" << "-"));

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

int ProcessXmlBuilder::firstPosTempNormal() const
{
    return 10;
}

int ProcessXmlBuilder::firstPosTempSearch() const
{
    return 2;
}

int ProcessXmlBuilder::firstPosTempScript() const
{
    return 13;
}

int ProcessXmlBuilder::firstPosTempOther() const
{
    return 7;
}

void ProcessXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == "name"
            || element == "ver"
            || element == "var"
            || element == "author"
            || element == "desc"
            || element == "uptime"
//            || element == "nowait"
            || element == "cmdc"
            || element == "localc"
            || element == "sep"
            || element == "file"
            /*|| element == "fstack"*/
            || element == "istack")
    {
        QStringList data;
        //add element and text
        data.append(element);
        QString text = rxml->readElementText();
        data.append(text);
        list->append(data);
    }

    if(element == "type")
    {
        QString only = rxml->attributes().value("only").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("only");
        data.append(only);

        //add to QList
        list->append(data);
    }

    if(element == "timeout")
    {
        QString during = rxml->attributes().value("dur").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("dur");
        data.append(during);

        //add to QList
        list->append(data);
    }

    if(element == "cmd" || element == "sname")
    {
        QString id = rxml->attributes().value("id").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("id");
        data.append(id);

        //add to QList
        list->append(data);
    }

    if(element == "lvar")
    {
        QString lval = rxml->attributes().value("lval").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("lval");
        data.append(lval);

        //add to QList
        list->append(data);
    }

    if(element == "output")
    {
        QString radio = rxml->attributes().value("radio").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("radio");
        data.append(radio);

        //add to QList
        list->append(data);
    }
}
