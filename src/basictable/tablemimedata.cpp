/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "tablemimedata.h"

TableMimeData::TableMimeData(QString mimename)
{
    classname = mimename;
}

void TableMimeData::setTableData(const QList<QStringList> list)
{
    if(classname.isEmpty()) return;
    setList(&list);
}

const QList<QStringList> TableMimeData::getTableData(QByteArray dat)
{
    QList<QStringList> result;
    QString dats = QString::fromLocal8Bit(dat);
    QStringList ilist = dats.split("\n\"", Qt::SkipEmptyParts);

    QStringList tmp;

    for(const auto &item : ilist){
        tmp.clear();
        tmp = item.split("\\\" \\\"");

        tmp.replace(0, tmp.first().remove(0,2));
        QString inner = tmp.last();
        tmp.replace(tmp.count() - 1, inner.left(inner.size() - 4));

        QStringList::iterator it;
        for (it = tmp.begin(); it != tmp.end(); ++it){
            (*it).replace("\\\\","\\");
        }

        result.append(tmp);
    }

    return result;
}

QStringList TableMimeData::formats() const
{
    return QStringList(getTableMimeType(classname));
}

bool TableMimeData::hasFormat(const QString &mimetype) const
{
    return (getTableMimeType(classname) == mimetype);
}

QVariant TableMimeData::retrieveData(const QString &mimetype
                                     , QMetaType preferredType) const
{
    return QMimeData::retrieveData(mimetype, preferredType);
}

void TableMimeData::setList(const QList<QStringList> *list)
{
    QString convert = "";

    for(const auto &ilist : *list){
        for(const auto &item : ilist){
            convert.append("\"").append(item).append("\" ");
        }
        convert.append("\n");
    }

    QByteArray array = escape(convert).toLocal8Bit();

    setData(this->getTableMimeType(classname), array);
}

QString TableMimeData::escape(QString plainText)
{
    QString result = plainText;
    result.replace("\\", "\\\\");
    result.replace("\"", "\\\"");
    result.replace("\t", "\", \"");
    result.replace("\n", "\"\n\"");
    return result;
}
