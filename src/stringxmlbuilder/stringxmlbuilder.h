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
    explicit StringXmlBuilder(QObject *parent = 0);
    ~StringXmlBuilder();

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    void copyItem(int itemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "string";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // STRINGXMLBUILDER_H
