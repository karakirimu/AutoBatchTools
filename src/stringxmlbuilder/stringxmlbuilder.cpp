#include "stringxmlbuilder.h"

StringXmlBuilder::StringXmlBuilder(QObject *)
{
    //set new folder
    QDir dir;
    dir.mkdir("./settings");

    //set new file
    setFileName("./settings/string");

    if(count() == 0){
        createDocument();
    }
}

StringXmlBuilder::~StringXmlBuilder()
{
}

bool StringXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool StringXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool StringXmlBuilder::insertItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::insertItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool StringXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool StringXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void StringXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

void StringXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, "variant");
}

bool StringXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void StringXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int StringXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void StringXmlBuilder::createVarElement(QList<QStringList> *internal, QStringList *var)
{
    internal->append(QStringList() << "variant" << var->at(0));
    internal->append(QStringList() << "value" << var->at(1));
}

void StringXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == "variant"
            || element == "value")
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }
}
