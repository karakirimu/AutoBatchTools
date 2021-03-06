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
    QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const override;

private:
    void setList(const QList<QStringList> *list);

    QString toCsv(QString plainText);

};

#endif // LISTMIMEDATA_H
