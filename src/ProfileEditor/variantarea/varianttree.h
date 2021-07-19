/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANTTREE_H
#define VARIANTTREE_H

#include <QObject>
#include <QTreeView>
#include <QWidget>
#include <QHeaderView>
#include <QMenu>
#include <QList>
#include <QDateTime>
#include <QClipboard>
#include <QFileDialog>
#include <QTreeWidget>
#include <QMessageBox>
#include <QDropEvent>

#include <editoperator.h>
#include <../stringxmlbuilder/stringxmlbuilder.h>

class VariantTree : public QTreeWidget {
    Q_OBJECT
public:
    explicit VariantTree(QWidget *parent = nullptr);
    ~VariantTree();

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
    void openFileAction();
    void openDirAction();
    void upAction();
    void downAction();

    void itemSelectUpdate();
    void itemDataUpdate(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    // undo redo no emit operation
    void insertRow(int row); //ui
    void insertRow(int row, int section); //from redo/undo
    void insertRow(int row, QModelIndex *index); //internal function
    void deleteRow(int row, int section);
    void deleteRowItems();

    void replaceItem(int row, QStringList strlist, int section);

    void swapRow(int before, int after);
    void swapRow(int before, int after, int section);
    void swapRow(int before, int after, QModelIndex *index);

    // popup
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    // keyboard shortcut
    bool eventFilter(QObject *obj, QEvent *event);
    void onCustomContextMenu(const QPoint &point);

    // drag and drop
    void dropEvent(QDropEvent *event);

    bool setLocalListItems(QTreeWidgetItem *parent, int itemid);
    bool setGlobalListItems(QTreeWidgetItem *parent);

    QStringList getLocalRowElement(int targetrow, int tablerow);
    QStringList getGlobalRowElement(int row);
    QStringList getEmptyVariants();
    QStringList getVariants(QTreeWidgetItem *childitem);

    void closeEditState();

    bool isParentValid();
    QModelIndex getSectionFromUi();

    //copied from BasicTable
    bool deleteCheckMessage();

    //model()->datachanged
    //like blocksignals
    void ignoreDataChangedSignal(bool valid);

    EditOperator *editop;
    StringXmlBuilder *builder;

    QAction *m_file;
    QAction *m_dir;
    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_cut;
    QAction *m_copy;
    QAction *m_paste;
    QAction *m_ref;
    QAction *m_up;
    QAction *m_down;

    QMenu *contextMenu;

    static const int LOCALINDEX = 1;

    // order of root element
    enum{TREE_GLOBAL,TREE_LOCAL/*,TREE_INTERNAL*/};
};

#endif // VARIANTTREE_H
