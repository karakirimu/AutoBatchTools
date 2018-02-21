#include "variantcombobox.h"

VariantComboBox::VariantComboBox(QObject *)
{
 sbuilder = new StringXmlBuilder();
 pbuilder = new ProcessXmlBuilder();
}

VariantComboBox::~VariantComboBox()
{
    delete sbuilder;
    delete pbuilder;
}

void VariantComboBox::reloadComboBoxItem()
{
    this->clear();
    QList<QStringList> item;
    int counter;

    //profileeditorxmlbuilder update (local)
    if(pbuilder->readItem(PEMAGIC, &item)){
        counter = QString(item.at(1).at(1)).toInt();
//        counter = VariantConverter::stringToInt();
        for(int i = 0; i < counter; i++){
            this->addItem(item.at(2 + i).at(1));
        }
    }
    item.clear();

    //stringbuilder update (global)
    counter = sbuilder->count();
    for(int i = 0; i < counter; i++){
        if(sbuilder->readItem(i, &item)){
            this->addItem(item.at(0).at(1));
            item.clear();
        }
    }
}

void VariantComboBox::setProfileFileName(QString filename)
{
    pbuilder->setLoadPath(filename);
}
