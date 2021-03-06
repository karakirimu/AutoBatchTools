/*
 * Copyright 2016-2021 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
    int count = ilist.count();
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

QVariant ListMimeData::retrieveData(const QString &mimetype, QVariant::Type preferredType) const
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
    int count = list->count();
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
