/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
 * @brief
 * Returns the file name saved in the XML file
 * according to the position of the combo box
 *
 * @param index The position of the combo box to retrieve.
 * @return filepath
 */
QString ProfileComboBox::getCurrentFileName(int index)
{
    if(index < 1) return "";

    QList<QStringList> item;

    // It needs to deduct this combobox's default text.
    builder->readItem(index - 1, &item);

    return item.at(2).at(1);
}

/**
 * @fn ProfileComboBox::setIndex
 * @brief Show an appropriate item from the file path.
 *        If it is not a proper file path, show the default item.
 *
 * @param filepath File path to set
 */
void ProfileComboBox::setIndex(QString filepath)
{
    QList<QStringList> item;
    QFileInfo info;
    QFileInfo other(filepath);
    int counter = builder->count();

    for(int i = 0; i < counter; i++){

        builder->readItem(i, &item);
        info.setFile(item.at(2).at(1));

        // info file exist && other file exist && same filepath
        if(info.exists() \
            && other.exists() \
            && info.canonicalFilePath() == other.canonicalFilePath()){
            this->setCurrentIndex(i + 1);
            break;
        }

        item.clear();
    }
}

/**
 * @fn ProfileComboBox::reloadComboBoxItem
 * @brief Read the xml file and update the combo box item.
 */
void ProfileComboBox::reloadComboBoxItem()
{
    this->clear();
    QFileInfo info;

    this->addItem(tr("Select profile ..."));

    QList<QList<QStringList> *> item;
    builder->readAll(&item);

    for(QList<QStringList> *index : item){
        info.setFile(xf.fetch(index, PROFILE_FILE));

        if(info.exists()){
            QString name = xf.fetch(index, PROFILE_NAME);
            name = name == "" ? tr("(no name)") : name;
            this->addItem(tr("%1 - %2").arg(name, info.fileName()));
        }else{
            this->addItem(tr("Unknown"));
        }
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
    QString file = dialog.getOpenFileName(this, tr("Add Profile")
                                          , "./"
                                          , tr("Profile ") + "(*.apro *.xml)");

    if(file == "") return;

    pbuilder->setLoadPath(file);

    //load info
    if(pbuilder->readItem(0, &item)){
        QStringList vl = (QStringList()
                          << xf.fetch(&item, pxc.TAG_I_NAME)
                          << xf.fetch(&item, pxc.TAG_I_DESCRIPTION)
                          << file);

        builder->createVarElement(&list, &vl);
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
 * @brief
 * Delete the combo box item and
 * delete the corresponding data in the XML file.
 */
void ProfileComboBox::deleteItemAction()
{
    int current = this->currentIndex();
    //if rowcount is zero.
    if(current < 1 || builder->count() == 0) return;

    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert")
                , tr("Do you want to exclude the selected item from the list?")
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
