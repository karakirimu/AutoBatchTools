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

#ifndef LIBBASICTABLE_H
#define LIBBASICTABLE_H

#include "basictable_global.h"
#include "basetable.h"
#include <QHeaderView>
#include <QMessageBox>

class BASICTABLESHARED_EXPORT BasicTable : public BaseTable
{
    Q_OBJECT
public:
    explicit BasicTable(QWidget *parent = Q_NULLPTR);
    ~BasicTable();

protected slots:
    virtual void editAction();
    virtual void addAction();
    virtual void deleteAction();
    virtual void copyAction();
    virtual void upAction();
    virtual void downAction();

    void horizontalHeaderClicked(int cols);

protected:
    virtual void setPopupActionTop();
    virtual void setPopupActionBottom();
    void setPopupActionDefault(QIcon copy, QIcon up, QIcon down);
    bool deleteCheckMessage();

    QAction *m_copy;
    QAction *m_up;
    QAction *m_down;
};

#endif // LIBBASICTABLE_H
