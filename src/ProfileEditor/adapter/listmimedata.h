/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef LISTMIMEDATA_H
#define LISTMIMEDATA_H

#include <QMimeData>
#include <editorcacheconverter.h>

#define LISTMIMEDATA_MIMETYPE QLatin1String("application/x-qt-profilerlist")

class ListMimeData : public QMimeData
{
    Q_OBJECT
public:
    ListMimeData();

    void setListData(const EditorCache list);

    static EditorCache getListData(const ListMimeData *lmime);

    QStringList formats() const override;
    bool hasFormat(const QString &mimetype) const override;

protected:
    virtual QVariant retrieveData(const QString &mimetype,
                          QMetaType preferredType) const override;

private:
    void setList(const QList<QStringList> *list);

    QString toCsv(QString plainText);

};

#endif // LISTMIMEDATA_H
