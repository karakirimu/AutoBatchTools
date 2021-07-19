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
    ~ProfileTable() override;

public slots:
    void newAction();
    void addAction()    override;
    void editAction()   override;
    void deleteAction() override;
    void upAction()     override;
    void downAction()   override;
    void reloadAction();

private slots:
    void launchEditor(int row, int col = 0);

private:
    void setPopupActionTop();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void createListFromXml(QString filename, QList<QStringList> *newlist);
    void createList(int row, QList<QStringList> *newlist);

    void updateXml();
    void setTableItem(int row, const QList<QStringList> *item);

    QAction *m_new;
    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    XmlResolver xf;
    ProcessXmlConstant pxc;
    ProfileXmlBuilder *builder;
};

#endif // PROFILETABLE_H
