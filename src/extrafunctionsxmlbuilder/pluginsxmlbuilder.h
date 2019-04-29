#ifndef EXTRAFUNCTIONSXMLBUILDER_H
#define EXTRAFUNCTIONSXMLBUILDER_H

#include "extrafunctionsxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>
#include <QDir>

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
    explicit ExtrafunctionsXmlBuilder(QObject *parent = nullptr);
    ~ExtrafunctionsXmlBuilder();

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    void copyItem(int itemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();

    const QString PL_NAME = "manual";
    const QString PL_ATTR_FILE = "file";

private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "plugins";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // EXTRAFUNCTIONSXMLBUILDER_H
