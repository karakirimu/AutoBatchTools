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

#ifndef SEARCHDIALOGTABLE_H
#define SEARCHDIALOGTABLE_H

//#include <basetable.h>
#include <../basictable/basictable.h>
#include <QWidget>
#include <QClipboard>
#include <QHeaderView>
#include <QApplication>
#include <../fileinfodialog/fileinfodialog.h>

class SearchDialogTable : public BasicTable
{
    Q_OBJECT
public:
    explicit SearchDialogTable(QWidget *parent = nullptr);
    ~SearchDialogTable();

private slots:
    void copyAction();
    void propertyAction();

private:
    void setPopupActionTop();

    bool eventFilter(QObject *obj, QEvent *event);

    QAction *m_property;
};

#endif // SEARCHDIALOGTABLE_H
