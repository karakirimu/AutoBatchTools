/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "listmimedata.h"

ListMimeData::ListMimeData()
{
}

void ListMimeData::setListData(const EditorCache list)
{
    EditorCacheConverter conv;
    QList<QStringList> result;
    conv.convertFromCache(&list, &result);
    setList(&result);
}

EditorCache ListMimeData::getListData(const ListMimeData *lmime)
{
    EditorCache result;
    if(!lmime->hasFormat(LISTMIMEDATA_MIMETYPE)) return result;

    QByteArray dat = lmime->data(LISTMIMEDATA_MIMETYPE);
    QString dats = QString::fromLocal8Bit(dat);
    QStringList ilist = dats.split("\n\"");

    QList<QStringList> encodedlist;
    QStringList tmp;
    qsizetype count = ilist.count();
    for(int i = 0; i < count - 1; i++){
        tmp.clear();
        tmp = static_cast<QString>(ilist.at(i)).split("\\\" \\\"");

        tmp.replace(0, static_cast<QString>(tmp.at(0)).remove(0,2));
        QString inner = static_cast<QString>(tmp.at(tmp.count() - 1));
        tmp.replace(tmp.count() - 1, inner.left(inner.size() - 4));

        QStringList::iterator it;
        for (it = tmp.begin(); it != tmp.end(); ++it){
            (*it).replace("\\\\","\\");
        }

        encodedlist.append(tmp);
    }

    EditorCacheConverter conv;
    conv.convertToCache(&result, &encodedlist);

    return result;
}

QStringList ListMimeData::formats() const
{
    return QStringList(LISTMIMEDATA_MIMETYPE);
}

QVariant ListMimeData::retrieveData(const QString &mimetype,
                                    QMetaType preferredType) const
{
    return QMimeData::retrieveData(mimetype, preferredType);
}

bool ListMimeData::hasFormat(const QString &mimetype) const
{
    return LISTMIMEDATA_MIMETYPE == mimetype ? true : false;
}

void ListMimeData::setList(const QList<QStringList> *list)
{
    QString convert = "";

    QStringList ilist;
    qsizetype count = list->count();
    for(int i = 0; i < count; i++){
        ilist = list->at(i);
        for(int j = 0; j < ilist.count(); j++){
            convert.append("\"").append(ilist.at(j)).append("\" ");
        }
        convert.append("\n");
    }

    QByteArray array = toCsv(convert).toLocal8Bit();

    setData(LISTMIMEDATA_MIMETYPE, array);
}

QString ListMimeData::toCsv(QString plainText)
{
    QString result = plainText;
    result.replace("\\", "\\\\");
    result.replace("\"", "\\\"");
    result.replace("\t", "\", \"");
    result.replace("\n", "\"\n\"");
    return result;
}
