/*
 * Copyright 2016-2021 karakirimu
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

#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

#include "searchtable_global.h"
#include "../basictable/basictable.h"
#include "../searchxmlbuilder/searchxmlbuilder.h"
#include "../filesearchdialog/filesearchdialog.h"

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
    void reloadAction();
    void copyAction() override;
    void upAction() override;
    void downAction() override;

private:
    void setPopupActionTop() override;
    void setPopupActionBottom() override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    SearchXmlBuilder *builder;
    void setTableItem(int row);
};

#endif // SEARCHTABLE_H
