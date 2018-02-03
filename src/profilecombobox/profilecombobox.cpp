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

int ProfileComboBox::getIndexFromFileName(QString filepath)
{
    QList<QStringList> item;
    int count = builder->count();
    for(int i = 0; i < count; i++){
        item.clear();
        if(builder->readItem(i, &item)
                && item.at(2).at(1) == filepath){
            return i;
        }
    }

    return -1;
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
            QString name = item.at(0).at(1);
            name = name == "" ? tr("(no name)") : name;
            this->addItem(tr("%1 - %2").arg(name).arg(info.fileName()));
        }else{
            this->addItem(tr("Unknown"));
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

    if(file == "") return;

    pbuilder->setLoadPath(file);

    //load info
    if(pbuilder->readItem(0, &item)){
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
