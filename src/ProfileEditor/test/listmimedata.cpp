#include "listmimedata.h"

ListMimeData::ListMimeData()
{
}

void ListMimeData::setList(const QList<QStringList> *list)
{
    //Windows
    //can't use foreach
    //convert to qvariant
//    QVariantList list_;
//    for(int i = 0; i < list->count(); i++){
//        list_.append(QVariant(list->at(i)));
//    }

    QString convert = "";
    QStringList ilist;
    int count = list->count();
    for(int i = 0; i < count; i++){
        ilist = list->at(i);
        for(int j = 0; j < ilist.count(); j++){
            convert.append("\"").append(ilist.at(j)).append("\" ");
        }
        convert.append("\n");
    }

//    foreach (QStringList ilist, list) {
//        foreach (QString cell, ilist) {
//            convert.append(cell).append(" ");
//        }
//        convert.append("\n");
//    }

    QByteArray array = toCsv(convert).toLocal8Bit();

    setData(QLatin1String("application/x-qt-profilerlist"), array);
}

//QList<QStringList> ListMimeData::list() const
//{
//    QVariant data = retrieveData(QLatin1String("application/x-qt-profilerlist"), QVariant::List);
//    QVariantList list = data.toList();
//    QList<QStringList> _list;
////    for(int i = 0; i < list.count(); i++){
////        _list.append(list.at(i).toStringList());
////    }

//    QMutableListIterator<QVariant> i(list);
//    while(i.hasNext()){
//        _list.append(i.next().toStringList());
//    }

//    return _list;
//}

QStringList ListMimeData::formats()
{
    return QStringList(QLatin1String("application/x-qt-profilerlist"));
}

QVariant ListMimeData::retrieveData(const QString &mimetype, QVariant::Type preferredType) const
{
    return QMimeData::retrieveData(mimetype, preferredType);
}

bool ListMimeData::hasFormat(const QString &mimetype) const
{
    return QLatin1String("application/x-qt-profilerlist") == mimetype ? true : false;
}

QString ListMimeData::toCsv(QString plainText)
{
    QString result = plainText;
    result.replace("\\", "\\\\");
    result.replace("\"", "\\\"");
    result.replace("\t", "\", \"");
    result.replace("\n", "\"\n\"");
//    result.prepend("\"");
//    result.append("\"");
    return result;
}
