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

class BASICTABLESHARED_EXPORT BaseTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit BaseTable(QWidget *parent = Q_NULLPTR);
    ~BaseTable();

protected slots:
    void onCustomContextMenu(const QPoint &point);
    void deleteTableRecursive();
    void copyTable(int index);

    QStringList selectFiles(QString basedir);
    QString selectFile(QString basedir);
    QString selectFolder(QString basedir);

protected:
    virtual void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    QStringList droppedFromOutside(QDropEvent *event);
    void droppedFromInside(QDropEvent *event);
    void swapTableRow(int from, int dest);
    void insertTableRow(int from, int dest);
    virtual bool eventFilter(QObject *obj, QEvent *event);

    QMenu *contextMenu;

};

#endif // BASETABLE_H
