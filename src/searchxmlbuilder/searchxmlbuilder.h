#ifndef SEARCHXMLBUILDER_H
#define SEARCHXMLBUILDER_H

#include "searchxmlbuilder_global.h"
#include <QObject>
#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class SearchSettings_xml
 * @brief QList<QStringList> short description
 * @param data
 * @details this function sets text and checkbox.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:      2:      3:      4:     5:
 * 0    :name     :text   :
 * 1    :variant  :text   :
 * 2    :keyword  :text   :
 * 3    :dir      :text   :
 * 4    :recursive:text   :
 * 5    :seconds  :text   :enabled:data   :
 * 6    :creation :text   :enabled:data   :combo   :data
 * 7    :modified :text   :enabled:data   :combo   :data
 * 8    :fsize_1  :text   :enabled:data   :combo   :data
 * 9    :fsize_2  :text   :enabled:data   :combo   :data
 */

class SEARCHXMLBUILDERSHARED_EXPORT SearchXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    SEARCHXMLBUILDERSHARED_EXPORT explicit SearchXmlBuilder(QObject *parent = 0);
    SEARCHXMLBUILDERSHARED_EXPORT ~SearchXmlBuilder();

    SEARCHXMLBUILDERSHARED_EXPORT bool readItem(int itemid, QList<QStringList> *itemlist);
    SEARCHXMLBUILDERSHARED_EXPORT bool addItem(const QList<QStringList> *itemlist);
    SEARCHXMLBUILDERSHARED_EXPORT bool deleteItem(int itemid);
    SEARCHXMLBUILDERSHARED_EXPORT bool editItem(int itemid, const QList<QStringList> *itemlist);
    SEARCHXMLBUILDERSHARED_EXPORT void swapItem(int beforeitemid, int afteritemid);
    SEARCHXMLBUILDERSHARED_EXPORT void copyItem(int itemid);
    SEARCHXMLBUILDERSHARED_EXPORT bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    SEARCHXMLBUILDERSHARED_EXPORT void createDocument();
    SEARCHXMLBUILDERSHARED_EXPORT int count();
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "search";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // SEARCHXMLBUILDER_H
