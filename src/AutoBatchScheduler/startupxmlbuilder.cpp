#include "startupxmlbuilder.h"

StartupXmlBuilder::StartupXmlBuilder(QObject *)
{
    //set new folder
    QDir dir;
    dir.mkdir("./settings");

    //set new file
    setFileName("./settings/startup");

    if(count() == 0){
        createDocument();
    }
}

StartupXmlBuilder::~StartupXmlBuilder()
{
}

bool StartupXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool StartupXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool StartupXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool StartupXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void StartupXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

void StartupXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, "name");
}

bool StartupXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void StartupXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int StartupXmlBuilder::count()
{
    return getSpecifiedElementItemsCount(FIRSTLAYER);
}

void StartupXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == "name"
            || element == "prof"
            || element == "valid"
            || element == "radio"
            || element == "schdt"
            || element == "schsec"
            || element == "scht"
            || element == "schday"
            || element == "unique")
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }
}
