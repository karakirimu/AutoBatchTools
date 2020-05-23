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

#ifndef EDITALLOWINPUT_H
#define EDITALLOWINPUT_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <uicommandmap.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
//#include <../variantconverter/variantconverter.h>
#include <editorcachelist.h>

class CheckEditAllowInput : public QUndoCommand
{
public:
//    CheckEditAllowInput(const int &targetindex
//                   , const bool &newcheck
//                   , QList<QList<QStringList> *> *cache
//                   , QUndoCommand *parent = nullptr);

    CheckEditAllowInput(const int &targetindex
                   , const bool &newcheck
                   , EditorCacheList *cache
                   , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:

    int index;
//    QString m_oldcheck;
//    QString m_newcheck;
//    QList<QList<QStringList> *> *m_cache;
//    ProcessXmlListGenerator pxlg;

    bool oldCheck;
    bool newCheck;

    EditorCacheList *ptrCache;
};

#endif // EDITALLOWINPUT_H
