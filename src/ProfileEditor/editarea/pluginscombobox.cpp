/*
 * Copyright 2016-2020 karakirimu
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

#include "pluginscombobox.h"

PluginsComboBox::PluginsComboBox(QObject *)
{
    //set new xml builder
    builder = new PluginsXmlBuilder();
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(pluginCheckAction(int)));

    searchsettings.append(QStringList() << "name" << "autoplug");
    searchsettings.append(QStringList() << "keyword" << "*.dll");
    searchsettings.append(QStringList() << "dir" << "./plugins");

}

PluginsComboBox::~PluginsComboBox()
{
    delete builder;
}


void PluginsComboBox::reloadComboBoxItem()
{
    // save selected position
    int tmp = this->currentIndex();

    //clear lists
    this->clear();
    buffer.clear();

    //read all auto list items
    FileSearchLoader fsload;
    buffer = fsload.searchFromStrList(&searchsettings);

    this->addItem(tr("Select plugin ..."));

    //read all manual list items
    QList<QStringList> item;
    int counter = builder->count();
    for(int i = 0; i < counter; i++){
        builder->readItem(i, &item);
        buffer.append(item.at(0).at(PATH_XML));
        item.clear();
    }

    //add item to list
    QMutableListIterator<QString> i(buffer);
    ExtraPluginInterface *inter;

    while(i.hasNext()){
        QPluginLoader loader(i.next());

        if(loader.load()){
            inter = qobject_cast<ExtraPluginInterface *>(loader.instance());
            this->addItem(inter->getInformation()->name);
            counter++;
        }
        loader.unload();
    }

    //reload selected position
    if(-1 < tmp && tmp < counter)
        this->setCurrentIndex(tmp);
}

void PluginsComboBox::setComboBoxItem(QString *filepath)
{
    QPluginLoader loader(*filepath);
    ExtraPluginInterface *inter;

    if(loader.load()){
        inter = qobject_cast<ExtraPluginInterface *>(loader.instance());
        if(int no = this->findText(inter->getInformation()->name) > -1){
            this->setCurrentIndex(no);
        }else{
            this->setCurrentIndex(0);
        }
        loader.unload();
    }else{
        this->setCurrentIndex(0);
    }
}

//DEPENDS_XML PLUGINS
void PluginsComboBox::addItemAction()
{
    //add empty item
    QList<QStringList> list;

    //add file
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);

#ifdef Q_OS_WIN
    QString extension = "(*.dll)";
#else
    QString extension = "(*.*)";
#endif
    QString file = dialog.getOpenFileName(this, tr("Select Plugin"), \
                                          QDir::currentPath(), tr("Plugin ") + extension );
    QFileInfo info(file);

    if(info.exists()){
        //check plugin can use
        QPluginLoader loader(file);
        if(loader.load()){
            QObject *plugin = loader.instance();
            ExtraPluginInterface *inter = qobject_cast<ExtraPluginInterface *>(plugin);
            list.append(QStringList () << builder->PL_XML_NAME << inter->getInformation()->name \
                        << builder->PL_XML_ATTR_FILE << file);
            builder->addItem(&list);

            //reload
            reloadComboBoxItem();
            this->setCurrentIndex(builder->count() - 1);
            loader.unload();

        }else{
            QMessageBox::warning(this, "AutoBatchEditor",
                                       tr("This dll file cannot be applied."),
                                       QMessageBox::Ok);
        }

    }
}

void PluginsComboBox::deleteAction()
{
    //if rowcount is zero.
    if(builder->count() == 0) return;

    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert"), tr("Do you want to delete selected item ?"), QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        //delete file item
        int current = this->currentIndex();
        builder->deleteItem(current - 1);

        //reload
        reloadComboBoxItem();

        //re-set combo select
        this->setCurrentIndex(current - 1);
    }
}

void PluginsComboBox::pluginCheckAction(int index)
{
    Q_UNUSED(index)

    QPluginLoader loader(getCurrentExtraFile());
    if(loader.load()){
        QObject *plugin = loader.instance();
        ExtraPluginInterface *inter = qobject_cast<ExtraPluginInterface *>(plugin);
        const PluginInformation *pinfo = inter->getInformation();

//        if(pinfo->hassettingwidget){
            emit pluginChanged(getCurrentExtraFile());
//        }

        QString desc = pinfo->tooltip;

        if(desc != ""){
            this->setToolTip(desc);
        }

        loader.unload();

    }else{
        emit pluginChanged("");
    }
}

/**
 * @fn PluginsComboBox::getCurrentExtraFile
 * @brief Gets the associated plugin file path from the combo box selection.
 * @return file path
 */
QString PluginsComboBox::getCurrentExtraFile()
{
    int selected = this->currentIndex();
    if(selected > 0){
        // -1 means avoid combobox default text.
        return buffer.at(selected - 1);
    }

    return "";
}
