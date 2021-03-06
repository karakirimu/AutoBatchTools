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

#ifndef VARIANTCOMBOBOX_H
#define VARIANTCOMBOBOX_H


#include <QComboBox>
#include <QWidget>
#include <editoperator.h>
#include <../processxmlbuilder/processxmlbuilder.h>
//#include <../stringxmlbuilder.h>
#include <../variantconverter/variantconverter.h>

class VariantComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit VariantComboBox(QObject *parent = nullptr);
    ~VariantComboBox();
    void reloadComboBoxItem();

    void setEditOperator(EditOperator *op);
//public slots:
//    void setProfileFileName(QString filename);

private:
    EditOperator *editop;
    StringXmlBuilder *sbuilder;
//    ProcessXmlBuilder *pbuilder;
//    static const int LOCALVARINDEX = 1;
};

#endif // VARIANTCOMBOBOX_H
