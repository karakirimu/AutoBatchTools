/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STRINGPROJECTVERSION_H
#define STRINGPROJECTVERSION_H

#include <QUndoCommand>
#include <editorcachelist.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <uicommandmap.h>

class StringProjectVersion : public QUndoCommand
{
public:
//    StringProjectVersion(const int &targetindex
//                         , QString newstring
//                         , QList<QList<QStringList> *> *cache
//                         , QUndoCommand *parent = nullptr);

    StringProjectVersion(const int &targetindex
                         , const QString &newstring
                         , EditorCacheList *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString newStr;
private:
    int index;
    QString oldStr;
//    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> posinfo;
//    ProcessXmlListGenerator pxlg;

    EditorCacheList *ptrCache;
};

#endif // STRINGPROJECTVERSION_H
