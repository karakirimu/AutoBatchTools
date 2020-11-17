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

#ifndef PROFILETABLE_H
#define PROFILETABLE_H

#include <QProcess>
#include <../basictable/basictable.h>
#include <../profilexmlbuilder/profilexmlbuilder.h>
#include <../processxmlbuilder/processxmlbuilder.h>
#include <../share/qss/qsspropertyconstant.h>

class ProfileTable : public BasicTable
{
    Q_OBJECT
public:
    explicit ProfileTable(QWidget *parent = nullptr);
    ~ProfileTable();

public slots:
    void newAction();
    void addAction();
    void editAction();
    void deleteAction();
    void upAction();
    void downAction();
    void reloadAction();
private:
    void setPopupActionTop();
    bool eventFilter(QObject *obj, QEvent *event);
    void createList(QString filename, QList<QStringList> *newlist);

    QAction *m_new;
    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    ProfileXmlBuilder *builder;
    void setTableItem(int row);
};

#endif // PROFILETABLE_H
