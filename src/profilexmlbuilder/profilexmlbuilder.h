#ifndef PROFILEXMLBUILDER_H
#define PROFILEXMLBUILDER_H

#include "profilexmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>


/**
 * @class The ProfileXmlBuilder class
 * @brief QList<QStringList> short description
 * @param data
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :name     :text   :
 * 1    :desc     :text   :
 * 2    :file     :text   :
 */
class PROFILEXMLBUILDERSHARED_EXPORT ProfileXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit ProfileXmlBuilder(QObject *parent = 0);
    ~ProfileXmlBuilder();

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "profilelist";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // PROFILEXMLBUILDER_H
