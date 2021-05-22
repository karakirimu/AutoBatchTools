/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef FILEQUEUETABLE_H
#define FILEQUEUETABLE_H

#include "filequeuetable_global.h"
#include "../basictable/basictable.h"
#include "../fileinfodialog/fileinfodialog.h"
#include <../share/qss/qsspropertyconstant.h>

class FILEQUEUETABLESHARED_EXPORT FileQueueTable : public BasicTable
{
    Q_OBJECT
public:
    explicit FileQueueTable(QWidget *parent = nullptr);
    ~FileQueueTable();

    void addFiles(const QStringList &filenames);

    void addFilesAction();
    void addFolderAction();

protected slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    void horizontalHeaderClicked(int column);
    void deleteAciton();
    void clearAction();
    void propertyAction();

private:
    QAction *m_addfile;
    QAction *m_adddir;
    QAction *m_deleteitems;
    QAction *m_deleteAll;
    QAction *m_property;
};

#endif // FILEQUEUETABLE_H
