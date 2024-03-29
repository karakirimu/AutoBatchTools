/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANTCOMBOBOX_H
#define VARIANTCOMBOBOX_H


#include <QComboBox>
#include <QWidget>
#include <editoperator.h>
#include <../processxmlbuilder/processxmlbuilder.h>
//#include <../stringxmlbuilder.h>
#include <../share/variantconverter.h>

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
