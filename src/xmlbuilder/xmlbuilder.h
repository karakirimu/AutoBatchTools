#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include "xmlbuilder_global.h"
#include "basexmlbuilder.h"
#include <QObject>

//two layers
class XMLBUILDERSHARED_EXPORT Xmlbuilder : public BaseXmlBuilder
{
    Q_OBJECT
protected:
    explicit Xmlbuilder(QObject *parent = nullptr);
    virtual ~Xmlbuilder();

    bool readItem(int itemid, QString firstlayername, QString attr, QList<QStringList> *itemlist);
    bool readAllItem(QString firstlayername, QString attr, QList<QList<QStringList> *> *itemlist);
    bool deleteItem(int itemid, QString firstlayername, QString attr);
    void swapItem(int beforeitemid, int afteritemid, QString rootelement, QString firstlayername, QString attr);
    bool overwriteItem(int itemid, QString rootelement, QString firstlayername, QString attr, const QList<QStringList> *itemlist);
    bool insertItem(int itemid, QString rootelement, QString firstlayername, QString attr, const QList<QStringList> *itemlist);

    void copyItem(int itemid, QString rootelement, QString firstlayername, QString attr, QString exchangedata);
    virtual void setSearchItemData(QString element, QList<QStringList> *list);

private:
    // This function starts with firstlayername and returns the entire string in an xml block
    // (e.g. <abc> </ abc>) containing the block defined in list. firstlayername can define the
    // initial value of indent with indentnum. Blocks below it are automatically indented.
    QString getStructureStr(int itemid, QString firstlayername, QString attr
                            , int indentnum, const QList<QStringList> *list, bool withparent);

    // This function returns only the line that needs to be updated by Id as a string.
//    QString getIdLine(int itemid, QString firstlayername, QString attr, int indentnum);

    qint64 getItemFirstLine(int tablenum, QString firstlayername, QString attr);

    void writeXmlItem(int itemid, QString firstlayername, QString attr, const QList<QStringList> *list);
    void writeElementData(const QList<QStringList> *list);

    bool insertItemId(int itemid, QString rootelement, QString firstlayername, QString attr, const QList<QStringList> *itemlist);

    bool deleteItemId(int destitemid, int fromitemid, QString firstlayername, QString attr);
    bool deleteItemIdRecursive(int deleteitemid, QString firstlayername, QString attr);

    int count(QString firstlayername);
};

#endif // XMLBUILDER_H
