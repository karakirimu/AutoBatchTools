/*
 * Copyright 2016-2019 karakirimu
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

/**
 * @fn ProfileComboBox::getCurrentFileName
 * @brief Extract the file name at the position selected by ui.
 * @return The filepath of the selected index.
 */
QString ProfileComboBox::getCurrentFileName()
{
    int selected = this->currentIndex();

    if(selected < 1) return "";

    return getCurrentFileName(selected);
}

/**
 * @fn ProfileComboBox::getCurrentFileName
 * @brief Returns the file name saved in the XML file according to the position of the combo box
 * @param index The position of the combo box to retrieve.
 * @return filepath
 */
QString ProfileComboBox::getCurrentFileName(int index)
{
    if(index < 1) return "";

    QList<QStringList> item;
    builder->readItem(index, &item);

    return item.at(2).at(1);
}

/**
 * @fn ProfileComboBox::getIndexFromFileName
 * @brief Get combo box position from filepath.
 * @param filepath
 * @return Combo box position or -1.
 */
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

/**
 * @fn ProfileComboBox::reloadComboBoxItem
 * @brief Read the xml file and update the combo box item.
 */
void ProfileComboBox::reloadComboBoxItem()
{
    this->clear();
    QList<QStringList> item;
    QFileInfo info;

    int counter = builder->count();

    this->addItem(tr("Select profile ..."));

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

/**
 * @fn ProfileComboBox::addItemAction
 * @brief Add combo box item and add data to XML file.
 * @remarks DEPENDS_XML
 */
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

/**
 * @fn ProfileComboBox::deleteItemAction
 * @brief Delete the combo box item and delete the corresponding data in the XML file.
 */
void ProfileComboBox::deleteItemAction()
{
    int current = this->currentIndex();
    //if rowcount is zero.
    if(current < 1 || builder->count() == 0) return;

    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert")
                , tr("Do you want to delete selected item ?\r\n(File is not delete)")
                , QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        //delete file item
        builder->deleteItem(current);

        //reload
        reloadComboBoxItem();

        //re-set combo select
        this->setCurrentIndex(current - 1);
    }
}
