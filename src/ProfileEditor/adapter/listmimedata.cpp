/*
 * Copyright 2016-2020 karakirimu
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

    setData(QLatin1String("application/x-qt-profilerlist"), array);
}

QStringList ListMimeData::formats() const
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
    return result;
}
