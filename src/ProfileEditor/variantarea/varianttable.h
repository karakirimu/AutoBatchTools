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
