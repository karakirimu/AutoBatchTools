#include "profilecombobox.h"

ProfileComboBox::ProfileComboBox(QObject *)
{
    builder = new ProfileXmlBuilder();
    pbuilder = new ProcessXmlBuilder();
}

ProfileComboBox::~ProfileComboBox()
{
    delete builder;
    delete pbuilder;
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

//DEPENDS_XML
void ProfileComboBox::addItemAction()
{
    //add empty item
    QList<QStringList> list;
    QList<QStringList> item;

    //add file
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString file = dialog.getOpenFileName(this, tr("Select Profile")
                                          , "./", tr("Profile (*.apro *.xml)"));

    pbuilder->setLoadPath(file);

    //load info
    if(pbuilder->readItem(0, &item) && item.count() == 6){
        list.append(QStringList() << "name" << item.at(1).at(1));
        list.append(QStringList() << "desc" << item.at(4).at(1));
        list.append(QStringList() << "file" << file);

        builder->addItem(&list);

        //reload
        reloadComboBoxItem();
        this->setCurrentIndex(builder->count() - 1);

    }else{
        if(file != ""){
            QMessageBox::warning(this, tr("Alert"),
                                       tr("File is not existed."),
                                       QMessageBox::Ok);
        }
    }
}

void ProfileComboBox::deleteItemAction()
{
    //if rowcount is zero.
    if(builder->count() == 0) return;

    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert")
                , tr("Do you want to delete selected item ?\r\n(File is not delete)")
                , QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        //delete file item
        int current = this->currentIndex();
        builder->deleteItem(current);

        //reload
        reloadComboBoxItem();

        //re-set combo select
        this->setCurrentIndex(current - 1);
    }
}
