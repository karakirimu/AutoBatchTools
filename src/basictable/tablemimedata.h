/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef TABLEMIMEDATA_H
#define TABLEMIMEDATA_H

#include <QMimeData>

constexpr char TABLEMIMETYPE_HEADER[] = "application/autobatchtools";

class TableMimeData : public QMimeData
{
    Q_OBJECT
public:
    TableMimeData(QString mimename);

    void setTableData(const QList<QStringList> list);
    static const QList<QStringList> getTableData(QByteArray dat);

    QStringList formats() const override;
    bool hasFormat(const QString &mimetype) const override;

    inline static const QString getTableMimeType(QString name){
        return QString("%1-%2").arg(TABLEMIMETYPE_HEADER, name);
    }

protected:
    virtual QVariant retrieveData(const QString &mimetype,
                                  QMetaType preferredType) const override;

private:
    void setList(const QList<QStringList> *list);

    QString escape(QString plainText);
    QString classname;
};

#endif // TABLEMIMEDATA_H
