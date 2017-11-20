#include "variantcombobox.h"

VariantComboBox::VariantComboBox(QObject *)
{
// sbuilder = new StringXmlBuilder();
// pbuilder = new ProcessXmlBuilder();
    editop = nullptr;
}

VariantComboBox::~VariantComboBox()
{
//    delete sbuilder;
//    delete pbuilder;
}

void VariantComboBox::reloadComboBoxItem()
{
    this->clear();
    QList<QStringList> item;
    int counter;

    //Processxmlbuilder update (local)
    if(editop->read(PEMAGIC, &item)){
        counter = VariantConverter::stringToInt(item.at(1).at(1));
        for(int i = 0; i < counter; i++){
            this->addItem(item.at(2 + i).at(1));
        }
    }
    item.clear();

    //stringbuilder update (global)
//    counter = sbuilder->count();
//    for(int i = 0; i < counter; i++){
//        if(sbuilder->readItem(i, &item)){
//            this->addItem(item.at(0).at(1));
//            item.clear();
//        }
    //    }
}

void VariantComboBox::setEditOperator(EditOperator *op)
{
    editop = op;
}

//void VariantComboBox::setProfileFileName(QString filename)
//{
//    pbuilder->setLoadPath(filename);
//}
