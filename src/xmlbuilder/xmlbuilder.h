#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include "xmlbuilder_global.h"
#include "basexmlbuilder.h"
#include <QObject>

class XMLBUILDERSHARED_EXPORT Xmlbuilder : public BaseXmlBuilder
{
    Q_OBJECT
protected:
    explicit Xmlbuilder(QObject *parent = 0);
    virtual ~Xmlbuilder();

    bool readItem(int itemid, QString firstlayername, QString attr, QList<QStringList> *itemlist);
    bool readAllItem(QString firstlayername, QString attr, QList<QList<QStringList> *> *itemlist);
    bool deleteItem(int itemid, QString firstlayername, QString attr);
    void swapItem(int beforeitemid, int afteritemid, QString rootelement, QString firstlayername, QString attr);
    bool overwriteItem(int itemid, QString rootelement, QString firstlayername, QString attr, const QList<QStringList> *itemlist);

    virtual void copyItem(int itemid, QString rootelement, QString firstlayername, QString attr, QString exchangedata);
    virtual void setSearchItemData(QString element, QList<QStringList> *list);

private:

    QString getAppendData(int itemid, QString firstlayername, QString attr, int indentnum, const QList<QStringList> *list, bool withparent);
    int getItemFirstLine(int tablenum, QString firstlayername, QString attr);
    void writeXmlItem(int itemid, QString firstlayername, QString attr, const QList<QStringList> *list);
    void writeElementData(const QList<QStringList> *list);
    bool shiftXmlData(int destitemid, int fromitemid, QString firstlayername, QString attr);
    bool shiftXmlDataRecursive(int deleteitemid, QString firstlayername, QString attr);
    int count(QString firstlayername);
};

#endif // XMLBUILDER_H
