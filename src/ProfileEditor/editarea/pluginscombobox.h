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

#ifndef PLUGINSCOMBOBOX_H
#define PLUGINSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QPluginLoader>
#include <QMessageBox>
#include <../pluginsxmlbuilder/pluginsxmlbuilder.h>
#include <../plugins/plugininterface/extraplugininterface.h>
#include <../filesearchloader/filesearchloader.h>
#include <QFileDialog>

class PluginsComboBox : public QComboBox
{
    Q_OBJECT

signals:
    void pluginChanged(QString);

public:
    explicit PluginsComboBox(QObject *parent = nullptr);
    ~PluginsComboBox();

    QString getCurrentExtraFile();
    void reloadComboBoxItem();
    void setComboBoxItem(QString *filepath);

public slots:
    void addItemAction();
    void deleteAction();
    void pluginCheckAction(int index);

private:
    PluginsXmlBuilder *builder;

    //file list buffer
    QStringList buffer;

    //auto search settings
    QList<QStringList> searchsettings;

    const int NAME_XML = 1;
    const int PATH_XML = 3;
};

#endif // PLUGINSCOMBOBOX_H
