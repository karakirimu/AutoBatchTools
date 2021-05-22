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
    ~BaseTable();

protected slots:
    void onCustomContextMenu(const QPoint &point);
    void deleteTableRecursive();
    void copyTable(int index);

    QString selectFile(QString basedir);
    QString selectFolder(QString basedir);

protected:
    virtual void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    QStringList droppedFromOutside(QDropEvent *event);
    void insideDropRowMove(QDropEvent *event);
    bool insideDropRowsMove(QDropEvent *event, QList<int> *selected = nullptr);
    void swapTableRow(int from, int dest);
    void insertTableRow(int from, int dest);
    virtual bool eventFilter(QObject *obj, QEvent *event);

    QMenu *contextMenu;

private:
    bool checkRowContinuous(QModelIndexList *indexes);

};

#endif // BASETABLE_H
