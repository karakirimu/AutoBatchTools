/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANTTABLE_H
#define VARIANTTABLE_H

#include <QObject>
#include <QWidget>
#include <editoperator.h>
#include <../basictable/basictable.h>

class VariantTable : public BasicTable
{
    Q_OBJECT
public:
    explicit VariantTable(QWidget *parent = nullptr);
    ~VariantTable();

    void setEditOperator(EditOperator *op);
    void updateIndex(QString operation);

    void reloadAction();

private slots:
    void addAction();
    void editAction();
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();

    void textEditedAction(int row, int column);

private:
    // undo redo no emit operation
    void insertItem(int row);
    void deleteItem(int row);
    void replaceItem(int row, QStringList strlist);
    void swapItem(int before, int after);

    // popup
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    // keyboard shortcut
    bool eventFilter(QObject *obj, QEvent *event);

    bool setLocalListItem(int itemid);
    void tableItemSwap(int from , int to);
    QStringList getTableData(int targetrow, int tablerow);
    QStringList getLocalVariants(int index);

    EditOperator *editop;

    QAction *m_new;
    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_cut;
    QAction *m_paste;
    QAction *m_ref;

    static const int LOCALINDEX = 1;
};

#endif // VARIANTTABLE_H
