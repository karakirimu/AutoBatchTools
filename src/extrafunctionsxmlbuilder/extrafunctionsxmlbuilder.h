#ifndef EXTRAFUNCTIONSXMLBUILDER_H
#define EXTRAFUNCTIONSXMLBUILDER_H

#include "extrafunctionsxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class ExtrafunctionsXmlBuilder
 * @brief QList<QStringList> short description
 * @param data
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :extras   :text   :
 */

class EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT ExtrafunctionsXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT explicit ExtrafunctionsXmlBuilder(QObject *parent = nullptr);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT ~ExtrafunctionsXmlBuilder();

    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT bool readItem(int itemid, QList<QStringList> *itemlist);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT bool addItem(const QList<QStringList> *itemlist);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT bool deleteItem(int itemid);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT bool editItem(int itemid, const QList<QStringList> *itemlist);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT void swapItem(int beforeitemid, int afteritemid);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT void copyItem(int itemid);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT void createDocument();
    EXTRAFUNCTIONSXMLBUILDERSHARED_EXPORT int count();
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "exfunc";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // EXTRAFUNCTIONSXMLBUILDER_H
