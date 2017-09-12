#ifndef STRINGXMLBUILDER_H
#define STRINGXMLBUILDER_H

#include "stringxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class StringSettings_xml
 * @brief QList<QStringList> short description
 * @param data
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :variant  :text   :
 * 1    :value    :text   :
 */

class STRINGXMLBUILDERSHARED_EXPORT StringXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    STRINGXMLBUILDERSHARED_EXPORT explicit StringXmlBuilder(QObject *parent = 0);
    STRINGXMLBUILDERSHARED_EXPORT ~StringXmlBuilder();

    STRINGXMLBUILDERSHARED_EXPORT bool readItem(int itemid, QList<QStringList> *itemlist);
    STRINGXMLBUILDERSHARED_EXPORT bool addItem(const QList<QStringList> *itemlist);
    STRINGXMLBUILDERSHARED_EXPORT bool deleteItem(int itemid);
    STRINGXMLBUILDERSHARED_EXPORT bool editItem(int itemid, const QList<QStringList> *itemlist);
    STRINGXMLBUILDERSHARED_EXPORT void swapItem(int beforeitemid, int afteritemid);
    STRINGXMLBUILDERSHARED_EXPORT void copyItem(int itemid);
    STRINGXMLBUILDERSHARED_EXPORT bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    STRINGXMLBUILDERSHARED_EXPORT void createDocument();
    STRINGXMLBUILDERSHARED_EXPORT int count();
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "string";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // STRINGXMLBUILDER_H
