/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "variantcombobox.h"

VariantComboBox::VariantComboBox(QObject *)
{
    sbuilder = new StringXmlBuilder();
// pbuilder = new ProcessXmlBuilder();
    editop = nullptr;
}

VariantComboBox::~VariantComboBox()
{
    delete sbuilder;
//    delete pbuilder;
}

void VariantComboBox::reloadComboBoxItem()
{
    this->clear();
    QList<QStringList> item;
    int counter;

    //this->addItem(tr("Select variable ..."));

    //stringbuilder update (global)
    counter = sbuilder->count();
    for(int i = 0; i < counter; i++){
        if(sbuilder->readItem(i, &item)){
            this->addItem(item.at(0).at(1));
            item.clear();
        }
    }

    //Variable cache update (Consider future XML changes) (local)
    FunctionType fs;
    QList<EditorCache> ec;
    editop->readAll(&ec);
    int index = 0;

    for(const EditorCache& ect : ec){
        if(fs.getType(ect.type) != fs.TYPE::LOCAL){
            break;
        }
        index++;
    }

    if(index == ec.count()) return;

    for (const VariantPair& pair : ec.at(index).local.variantData) {
        this->addItem(pair.variant);
    }

}

void VariantComboBox::setEditOperator(EditOperator *op)
{
    editop = op;
}

//void VariantComboBox::setProfileFileName(QString filename)
//{
//    pbuilder->setLoadPath(filename);
//}
