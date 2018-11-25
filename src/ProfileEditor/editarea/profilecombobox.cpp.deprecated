#include "profilecombobox.h"

ProfileComboBox::ProfileComboBox(QObject *)
{
    builder = new ProfileXmlBuilder();
}

ProfileComboBox::~ProfileComboBox()
{
    delete builder;
}

QString ProfileComboBox::getCurrentFileName()
{
    int selected = this->currentIndex();
    return getCurrentFileName(selected);
}

QString ProfileComboBox::getCurrentFileName(int index)
{
    if(index > -1){
        QList<QStringList> item;
        builder->readItem(index, &item);
        return item.at(2).at(1);
    }

    return "";
}

void ProfileComboBox::reloadComboBoxItem()
{
    this->clear();
    QList<QStringList> item;
    QFileInfo info;
    int counter = builder->count();
    for(int i = 0; i < counter; i++){
        builder->readItem(i, &item);
        info.setFile(item.at(2).at(1));
        if(info.exists()){
            this->addItem(item.at(0).at(1) + " - " + info.fileName());
        }else{
            this->addItem(item.at(0).at(1) + " - Invalid");
        }
        item.clear();
    }
}
