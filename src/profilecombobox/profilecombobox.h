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

#ifndef PROFILECOMBOBOX_H
#define PROFILECOMBOBOX_H

#include "profilecombobox_global.h"

#include <QComboBox>
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <../profilexmlbuilder/profilexmlbuilder.h>
#include <../processxmlbuilder/processxmlbuilder.h>

class PROFILECOMBOBOXSHARED_EXPORT ProfileComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ProfileComboBox(QObject *parent = nullptr);
    ~ProfileComboBox();

    QString getCurrentFileName();
    QString getCurrentFileName(int index);
//    int getIndexFromFileName(QString filepath);

    void setIndex(QString filepath);

public slots:
    void reloadComboBoxItem();
    void addItemAction();
    void deleteItemAction();

private:
    ProfileXmlBuilder *builder;
    ProcessXmlBuilder *pbuilder;

};

#endif // PROFILECOMBOBOX_H
