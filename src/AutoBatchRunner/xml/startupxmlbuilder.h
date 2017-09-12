#ifndef STARTUPXMLBUILDER_H
#define STARTUPXMLBUILDER_H

//#include "twolayersxmlbuilder.h"
#include <../xmlbuilder/xmlbuilder.h>
/**
 * @class StartupSettings_xml
 * @brief QList<QStringList> short description
 * @param data
 * @details this function sets text and checkbox.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :name     :text   :
 * 1    :prof     :text   :
 * 2    :valid    :text   :
 * 3    :radio    :text   ://radio button position
 * 4    :schdt    :text   ://schedule datetime
 * 5    :schsec   :text   ://schedule seconds
 * 6    :scht     :text   ://schedule time
 * 7    :schday   :text   ://schedule day
 * 8    :unique   :text   ://tray managing number
 *
 * schday data structure
 * weeks: 1 or bigger than 1 (0 is invalid)
 * day : enabled = 1, disabled = 0
 * weeks every :monday :tuesday :wednesday :thursday :friday :saturday :sunday :
 *
 * example:
 * 20011010
 * 2:0:0:1:1:0:1:0:
 * every weeks = 2, mon = 0, tue = 0, wed = 1, thu = 1, fri = 0, sat = 1, sun = 0
 */

class StartupXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit StartupXmlBuilder(QObject *parent = 0);
    ~StartupXmlBuilder();

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

    const QString ROOTELEMENT = "startup";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // STARTUPXMLBUILDER_H
