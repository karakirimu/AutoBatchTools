/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

#include "searchtable_global.h"
#include "../basictable/basictable.h"
#include "../searchxmlbuilder/searchxmlbuilder.h"
#include "../filesearchdialog/filesearchdialog.h"
#include <../processxmllistgenerator/processxmllistgenerator.h>

class SEARCHTABLESHARED_EXPORT SearchTable : public BasicTable
{
    Q_OBJECT
public:
    explicit SearchTable(QWidget *parent = nullptr);
    virtual ~SearchTable() override;

public slots:
    void addAction() override;
    void editAction() override;
    void deleteAction() override;
    void copyAction() override;
    void upAction() override;
    void downAction() override;
    void reloadAction();

private slots:
    void launchSearchSetting(int row, int col = 0);

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event) override;

    void setTableItem(int row);
    void setTableItem(int row, const QList<QStringList> *item);

    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    SearchXmlBuilder *builder;
    ProcessXmlListGenerator xf;
};

#endif // SEARCHTABLE_H
