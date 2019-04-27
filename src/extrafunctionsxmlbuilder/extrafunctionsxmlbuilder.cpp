#include "extrafunctionsxmlbuilder.h"


ExtrafunctionsXmlBuilder::ExtrafunctionsXmlBuilder(QObject *parent)
    : Xmlbuilder(parent)
{
    //set new folder
    QDir dir;
    dir.mkdir("./plugins");

    //set new file
    setFileName("./plugins/.plist");

    if(count() == 0){
        createDocument();
    }
}

ExtrafunctionsXmlBuilder::~ExtrafunctionsXmlBuilder()
{

}

bool ExtrafunctionsXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool ExtrafunctionsXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool ExtrafunctionsXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool ExtrafunctionsXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void ExtrafunctionsXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

void ExtrafunctionsXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, PL_NAME);
}

bool ExtrafunctionsXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void ExtrafunctionsXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int ExtrafunctionsXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void ExtrafunctionsXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == PL_NAME)
    {
//        QStringList data;
//        //add element and text
//        data.append(element);
//        data.append(rxml->readElementText());
//        list->append(data);

        list->append(QStringList() << element << rxml->readElementText()
                     << PL_ATTR_FILE << rxml->attributes().value(PL_ATTR_FILE).toString());
    }
}
