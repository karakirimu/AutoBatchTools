/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef BASETABLE_H
#define BASETABLE_H

#include "basictable_global.h"
#include <QTableWidget>
#include <QMenu>
#include <QWidget>
#include <QFileDialog>
#include <QMimeData>
#include <QMouseEvent>
#include <QDebug>
#include <QHeaderView>
#include <QDropEvent>

class BASICTABLESHARED_EXPORT BaseTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit BaseTable(QWidget *parent = nullptr);
    virtual ~BaseTable() override;

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    void deleteTableRecursive();
    void deleteTableRecursive(std::function<void(int)> predelete);

    void swapTableRow(int from, int dest);

    QStringList droppedFromOutside(QDropEvent *event);
    [[deprecated]] bool insideDropRowsMove(QDropEvent *event
                                           , QList<int> *selected = nullptr);
    bool droppedFromInside(int droppedrow, QList<int> *selected = nullptr);

    QMenu *contextMenu;

private:
    bool checkRowContinuous(QModelIndexList *indexes);

};

#endif // BASETABLE_H
