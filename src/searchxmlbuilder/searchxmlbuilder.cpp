#include "searchxmlbuilder.h"

SearchXmlBuilder::SearchXmlBuilder(QObject *)
{
    //set new file
    setFileName("./searchsettings.xml");

    if(count() == 0){
        createDocument();
    }
}

SearchXmlBuilder::~SearchXmlBuilder()
{
}

bool SearchXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool SearchXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool SearchXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool SearchXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void SearchXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

// this function needs to call setrowcount
void SearchXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, "name");
}

bool SearchXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void SearchXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int SearchXmlBuilder::count()
{
    return getSpecifiedElementItemsCount(FIRSTLAYER);
}

/**
 * @brief SearchXmlBuilder::getItemData
 * @param element
 * @param list
 * @details
 * QList<QStringList> structures
 * QList
 * |
 * +[0] QStringList <- inserted order as follows. [elementname, value, attr1, attr1_value, attr2,...]
 * +[1] QStringList
 *
 * this functions getting set element child only.
 */
void SearchXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == "name"
            || element == "variant"
            || element == "keyword"
            || element == "dir"
            || element == "recursive")
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }

    if(element == "seconds")
    {
        QString enabled = rxml->attributes().value("enabled").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("enabled");
        data.append(enabled);

        //add to QList
        list->append(data);
    }

    if(element == "fsize_1"
            || element == "fsize_2"
            || element == "creation"
            || element == "modified")
    {
        QString enabled = rxml->attributes().value("enabled").toString();
        QString order = rxml->attributes().value("combo").toString();
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        //add attributes and data
        data.append("enabled");
        data.append(enabled);
        data.append("combo");
        data.append(order);

        //add to QList
        list->append(data);
    }

    //qDebug() << "//" << element << "//";
}
