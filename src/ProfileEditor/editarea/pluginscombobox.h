/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PLUGINSCOMBOBOX_H
#define PLUGINSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QPluginLoader>
#include <QMessageBox>
#include <../pluginsxmlbuilder/pluginsxmlbuilder.h>
#include <../plugininterface/extraplugininterface.h>
#include <../filesearchloader/filesearchloader.h>
#include <QFileDialog>

class PluginsComboBox : public QComboBox
{
    Q_OBJECT

signals:
    void pluginChanged(QString);

public:
    explicit PluginsComboBox(QObject *parent = nullptr);
    ~PluginsComboBox() override;

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
    qsizetype autodetectcounter;

    //auto search settings
    QList<QStringList> searchsettings;

    const int NAME_XML = 1;
    const int PATH_XML = 3;
};

#endif // PLUGINSCOMBOBOX_H
