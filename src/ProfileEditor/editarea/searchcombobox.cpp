#include "searchcombobox.h"

SearchComboBox::SearchComboBox(QObject *)
{
    //set new xml builder
    builder = new SearchXmlBuilder();
}

SearchComboBox::~SearchComboBox()
{
    delete builder;
}

void SearchComboBox::reloadComboBoxItem()
{
    int tmp = this->currentIndex();
    this->clear();
    QList<QStringList> item;
    int counter = builder->count();

    this->addItem(tr("Select filesearch condition ..."));

    for(int i = 0; i < counter; i++){
        if(builder->readItem(i, &item)){
            this->addItem(builder->fetch(SEARCH_NAME,SEARCH_NONE, &item));
            item.clear();
        }
    }
    if(-1 < tmp && tmp < counter)
        this->setCurrentIndex(tmp);
}

void SearchComboBox::addAction()
{
    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
//    setTheme(fs);
    fs->setWindowTitle(tr("Editing - Untitled*"));
    if(fs->exec() == QDialog::Accepted){
        reloadComboBoxItem();
        this->setCurrentIndex(this->count() - 1);
    }
}

void SearchComboBox::editAction()
{
    //if count is one
    if(this->count() == 1) return;

    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
//    setTheme(fs);
    QList<QStringList> list;

    int index = this->currentIndex() - 1;

    if(index < 0) return;

    if(builder->readItem(index, &list)){
        //set title
        fs->loadSettingList(index, &list);
    }
    if(fs->exec() == QDialog::Accepted){
        reloadComboBoxItem();
    }
}

void SearchComboBox::deleteAction()
{
    //if rowcount is zero.
    if(builder->count() == 0) return;

    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert")
                , tr("Do you want to delete selected item?")
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

//QString SearchComboBox::getCurrentVariant()
//{
//    int selected = this->currentIndex();
//    if(selected > -1){
//        QList<QStringList> item;
//        builder->readItem(selected, &item);
//        return item.at(1).at(1);
//    }

//    return "";
//}
