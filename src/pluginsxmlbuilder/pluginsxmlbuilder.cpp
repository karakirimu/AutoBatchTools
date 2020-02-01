#include "pluginsxmlbuilder.h"


PluginsXmlBuilder::PluginsXmlBuilder(QObject *parent)
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

PluginsXmlBuilder::~PluginsXmlBuilder()
{

}

bool PluginsXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool PluginsXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool PluginsXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool PluginsXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void PluginsXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

void PluginsXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, PL_XML_NAME);
}

bool PluginsXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void PluginsXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int PluginsXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void PluginsXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == PL_XML_NAME)
    {
//        QStringList data;
//        //add element and text
//        data.append(element);
//        data.append(rxml->readElementText());
//        list->append(data);

        list->append(QStringList() << element << rxml->readElementText()
                     << PL_XML_ATTR_FILE << rxml->attributes().value(PL_XML_ATTR_FILE).toString());
    }
}
