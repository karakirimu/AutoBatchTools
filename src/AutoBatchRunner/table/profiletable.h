/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
