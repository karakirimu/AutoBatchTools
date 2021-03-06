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

    //this->addItem(tr("Select variant ..."));

    //stringbuilder update (global)
    counter = sbuilder->count();
    for(int i = 0; i < counter; i++){
        if(sbuilder->readItem(i, &item)){
            this->addItem(item.at(0).at(1));
            item.clear();
        }
    }

    //Variant cache update (Consider future XML changes) (local)
    FunctionType fs;
    QList<EditorCache> ec;
    editop->readAll(&ec);
    int index = 0;

    for(EditorCache ect : ec){
        if(fs.getType(ect.type) != fs.TYPE::LOCAL){
            break;
        }
        index++;
    }

    if(index == ec.count()) return;

    for (VariantPair pair : ec.at(index).local.variantData) {
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
