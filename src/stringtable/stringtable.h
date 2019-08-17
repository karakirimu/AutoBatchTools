/*
 * Copyright 2016-2019 karakirimu
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

#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include <QClipboard>
#include <QApplication>

#include "stringtable_global.h"
#include "../basictable/basictable.h"
#include "../stringxmlbuilder/stringxmlbuilder.h"

class STRINGTABLESHARED_EXPORT StringTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StringTable(QWidget *parent = Q_NULLPTR);
    ~StringTable();

public slots:
    void addAction();
    void editAction();
    void deleteAction();
    void reloadAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();
    void openFileAction();
    void openDirectoryAction();

    void saveAction(int row);
    void resave();

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();
    void setTableItem(int row);
    bool eventFilter(QObject *obj, QEvent *event);
    void createList(int row, QList<QStringList> *newlist);

    QAction *m_file;
    QAction *m_dir;
    QAction *m_add;
    QAction *m_delete;
    QAction *m_edit;
    QAction *m_cut;
    QAction *m_paste;
    QAction *m_ref;

    StringXmlBuilder *builder;
};

#endif // STRINGTABLE_H
