#include "scriptcombobox.h"

ExtraFunctionsComboBox::ExtraFunctionsComboBox(QObject *)
{
    //set new xml builder
    builder = new ExtrafunctionsXmlBuilder();
}

ExtraFunctionsComboBox::~ExtraFunctionsComboBox()
{
    delete builder;
}

void ExtraFunctionsComboBox::reloadComboBoxItem()
{
    this->clear();
    QList<QStringList> item;
    QFileInfo info;
    int counter = builder->count();
    for(int i = 0; i < counter; i++){
        builder->readItem(i, &item);
        info.setFile(item.at(1).at(1));
        this->addItem(info.fileName());
        item.clear();
    }
}

void ExtraFunctionsComboBox::editAction(QString variant)
{
    int selected = this->currentIndex();
    if(selected > -1){
        QList<QStringList> item;
        QList<QStringList> newitem;
        builder->readItem(selected, &item);

        //update item string
        newitem.append((QStringList () << "variant" << variant));
        newitem.append((QStringList () << "jsfile" << item.at(1).at(1)));

        //edit
        builder->editItem(selected, &newitem);

        //clear
        item.clear();
        newitem.clear();
    }
}

QString ExtraFunctionsComboBox::getCurrentVariant()
{
    int selected = this->currentIndex();
    if(selected > -1){
        QList<QStringList> item;
        builder->readItem(selected, &item);
        return item.at(0).at(1);
    }

    return "";
}
