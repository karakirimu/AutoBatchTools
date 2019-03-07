#include "pluginscombobox.h"

PluginsComboBox::PluginsComboBox(QObject *)
{
    //set new xml builder
    builder = new ExtrafunctionsXmlBuilder();
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(pluginCheckAction(int)));
}

PluginsComboBox::~PluginsComboBox()
{
    delete builder;
}

void PluginsComboBox::reloadComboBoxItem()
{
    int tmp = this->currentIndex();
    this->clear();
    QList<QStringList> item;
    QFileInfo info;
    int counter = builder->count();
    for(int i = 0; i < counter; i++){
        builder->readItem(i, &item);
        info.setFile(item.at(0).at(1));
        this->addItem(info.baseName());
        item.clear();
    }
    if(-1 < tmp && tmp < counter)
        this->setCurrentIndex(tmp);
}

//DEPENDS_XML PLUGINS
void PluginsComboBox::addItemAction()
{
    //add empty item
    QList<QStringList> list;

    //add file
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString file = dialog.getOpenFileName(this, tr("Select Plugin"), \
                                          QDir::currentPath(), tr("dll (*.dll)"));
    QFileInfo info(file);

    if(info.exists()){
        //check plugin can use
        QPluginLoader loader(file);
        if(loader.load()){
            QObject *plugin = loader.instance();
            if(qobject_cast<ExtraPluginInterface *>(plugin)){
                list.append(QStringList () << "extras" << file);
                builder->addItem(&list);

                //reload
                reloadComboBoxItem();
                this->setCurrentIndex(builder->count() - 1);

            }
            loader.unload();

        }else{
            QMessageBox::warning(this, tr("ProfileEditor"),
                                       tr("This dll file can not be applied."),
                                       QMessageBox::Ok);
        }

    }
}

//void ScriptComboBox::editAction(QString variant)
//{
//    int selected = this->currentIndex();
//    if(selected > -1){
//        QList<QStringList> item;
//        QList<QStringList> newitem;
//        builder->readItem(selected, &item);

//        //update item string
//        newitem.append((QStringList () << "extras" << item.at(0).at(1)));
////        newitem.append((QStringList () << "jsfile" << item.at(1).at(1)));

//        //edit
//        builder->editItem(selected, &newitem);

//        //clear
//        item.clear();
//        newitem.clear();
//    }
//}

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
        builder->deleteItem(current);

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
        if(inter->existsSettingWidget()){
            emit pluginChanged(true);
        }else{
            emit pluginChanged(false);
        }

        QString desc = inter->tooltipString();

        if(desc != ""){
            this->setToolTip(desc);
        }

        loader.unload();

    }
}

QString PluginsComboBox::getCurrentExtraFile()
{
    int selected = this->currentIndex();
    if(selected > -1){
        QList<QStringList> item;
        builder->readItem(selected, &item);
        return item.at(0).at(1);
    }

    return "";
}
