#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include "xmlbuilder_global.h"
#include "basexmlbuilder.h"
#include <QObject>

class XMLBUILDERSHARED_EXPORT Xmlbuilder : public BaseXmlBuilder
{
    Q_OBJECT
protected:
    XMLBUILDERSHARED_EXPORT explicit Xmlbuilder(QObject *parent = 0);
    XMLBUILDERSHARED_EXPORT virtual ~Xmlbuilder();

    XMLBUILDERSHARED_EXPORT bool readItem(int itemid, QString firstlayername, QString attr, QList<QStringList> *itemlist);
    XMLBUILDERSHARED_EXPORT bool deleteItem(int itemid, QString firstlayername, QString attr);
    XMLBUILDERSHARED_EXPORT void swapItem(int beforeitemid, int afteritemid, QString rootelement, QString firstlayername, QString attr);
    XMLBUILDERSHARED_EXPORT bool overwriteItem(int itemid, QString rootelement, QString firstlayername, QString attr, const QList<QStringList> *itemlist);

    XMLBUILDERSHARED_EXPORT virtual void copyItem(int itemid, QString rootelement, QString firstlayername, QString attr, QString exchangedata);
    XMLBUILDERSHARED_EXPORT virtual void setSearchItemData(QString element, QList<QStringList> *list);

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
