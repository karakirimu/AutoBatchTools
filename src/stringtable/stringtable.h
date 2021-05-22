/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include <QClipboard>
#include <QApplication>

#include "stringtable_global.h"
#include "../basictable/basictable.h"
#include "../stringxmlbuilder/stringxmlbuilder.h"
#include <../share/qss/qsspropertyconstant.h>

class STRINGTABLESHARED_EXPORT StringTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StringTable(QWidget *parent = nullptr);
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
