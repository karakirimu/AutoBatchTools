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
    ~StringTable() override;

public slots:
    void addAction()     override;
    void editAction()    override;
    void deleteAction()  override;
    void cutAction();
    void copyAction()    override;
    void pasteAction();
    void upAction()      override;
    void downAction()    override;
    void openFileAction();
    void openDirectoryAction();
    void reloadAction();

private slots:
    void saveCellChanged(int row, int col);

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    void updateXml();
    void setTableItem(int row, const QList<QStringList> *item);
    void createList(int row, QList<QStringList> *newlist);
    bool eventFilter(QObject *obj, QEvent *event) override;

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
