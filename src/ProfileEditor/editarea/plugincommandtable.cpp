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

#include "plugincommandtable.h"

PluginCommandTable::PluginCommandTable(QWidget *parent)
    : CommandTable(parent)
{

}

PluginCommandTable::~PluginCommandTable()
{

}

/**
 * @fn PluginCommandTable::updateTableList
 * @brief The process of replacing all the contents of the table.
 */
void PluginCommandTable::updateTableList(QStringList *list)
{
    int counter = list->count();
    clearContents();
    setRowCount(counter);

    for(int i = 0; i < counter; i++){
       this->blockSignals(true);
       this->setItem(i, 0, new QTableWidgetItem(list->at(i)));
       this->blockSignals(false);
    }
}

/**
 * @fn PluginCommandTable::insertSettingUpdate
 * @brief A slot that receives as an argument the UI state
 *        change of the widget created by the plugin.
 * @param list Argument list updated from plugin widget.
 */
void PluginCommandTable::insertSettingUpdate(QStringList list)
{
    clearContents();

    int counter = list.count();
    setRowCount(counter);

    for(int i = 0; i < counter; i++){
       this->blockSignals(true);
       this->setItem(i, 0, new QTableWidgetItem(list.at(i)));
       this->blockSignals(false);
    }

    emit updatePluginWidget(list, ProcessXmlListGenerator::TABLE_ALLUPDATE);
}
