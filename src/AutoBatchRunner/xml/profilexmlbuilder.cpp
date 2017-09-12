#include "profilexmlbuilder.h"

ProfileXmlBuilder::ProfileXmlBuilder(QObject *)
{
    //set new file
    setFileName("./profilelist.xml");

    if(count() == 0){
        createDocument();
    }
}

ProfileXmlBuilder::~ProfileXmlBuilder()
{

}

bool ProfileXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool ProfileXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool ProfileXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool ProfileXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void ProfileXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

bool ProfileXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void ProfileXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int ProfileXmlBuilder::count()
{
    return getSpecifiedElementItemsCount(FIRSTLAYER);
}

void ProfileXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == "name"
            || element == "desc"
            || element == "file")
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }
}
