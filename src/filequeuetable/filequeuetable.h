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

#ifndef FILEQUEUETABLE_H
#define FILEQUEUETABLE_H

#include "filequeuetable_global.h"
#include "../basictable/basictable.h"
#include "../fileinfodialog/fileinfodialog.h"

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
