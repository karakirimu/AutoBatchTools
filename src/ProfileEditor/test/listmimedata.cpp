#include "listmimedata.h"

ListMimeData::ListMimeData()
{
}

void ListMimeData::setList(const QList<QStringList> &list)
{
    //convert to qvariant
    QVariantList list_;
    for(int i = 0; i < list.count(); i++){
        list_.append(QVariant(list.at(i)));
    }

    setData(QLatin1String("application/apro"), QVariant(list_).toByteArray());
}

QList<QStringList> ListMimeData::list() const
{
    QVariant data = retrieveData(QLatin1String("application/apro"), QVariant::List);
    QVariantList list = data.toList();
    QList<QStringList> _list;
//    for(int i = 0; i < list.count(); i++){
//        _list.append(list.at(i).toStringList());
//    }

    QMutableListIterator<QVariant> i(list);
    while(i.hasNext()){
        _list.append(i.next().toStringList());
    }

    return _list;
}

QStringList ListMimeData::formats()
{
    return QStringList(QLatin1String("application/apro"));
}

QVariant ListMimeData::retrieveData(const QString &mimetype, QVariant::Type preferredType) const
{
    return QMimeData::retrieveData(mimetype, preferredType);
}

bool ListMimeData::hasFormat(const QString &mimetype) const
{
    return QLatin1String("application/apro") == mimetype ? true : false;
}
