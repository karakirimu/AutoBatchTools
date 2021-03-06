/*
 * Copyright 2016-2021 karakirimu
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

    emit updatePluginWidget(list, UiCommandMap::PL_ALLUPDATE_TABLE);
}

void PluginCommandTable::addAction()
{
    int row = this->rowCount();
    this->insertRow(row);
    emit updateTable(row, "", UiCommandMap::PL_ADD_TABLE);

    //for useability
    this->setCurrentItem(itemAt(row,0));
    this->selectRow(row);
}

void PluginCommandTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(!deleteCheckMessage()) return;

    QModelIndexList lists = this->selectedIndexes();

    while(!lists.empty()){
        this->removeRow(lists.at(0).row());
        emit updateTable(lists.at(0).row(), "", UiCommandMap::PL_DELETE_TABLE);

        lists = this->selectedIndexes();
    }
}

void PluginCommandTable::cutAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();
    int rows = mlist.count();
    for(int i = 0; i < rows; i++){
        tmp.append(mlist.at(i).data().toString());
        if(i < rows) tmp.append("\t");

        emit updateTable(mlist.at(i).row(), "", UiCommandMap::PL_CUT_TABLE);
    }

    //force delete
    BaseTable::deleteTableRecursive();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);
}

void PluginCommandTable::pasteAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split("\t");

    //last lests unknown ""
    if(text.last() == "") text.removeLast();

    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
        if(row > 0) row = this->currentRow();
        insertRow(row);
        this->blockSignals(true);
        this->setItem(row, 0, new QTableWidgetItem(text.at(i)));
        this->blockSignals(false);
        emit updateTable(row, text.at(i), UiCommandMap::PL_PASTE_TABLE);
    }
}

void PluginCommandTable::pasteSpaceAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(QRegularExpression("\\t| "));

    if(text.last() == "") text.removeLast();
    if(text.first() == "") text.removeFirst();

    int txcount = text.count();
    int row = this->currentRow();
    row = (row < 0) ? 0 : row;

    for(int i = 0; i < txcount; i++){
        insertRow(row);
    }

    for(int i = 0; i < txcount; i++){
        this->blockSignals(true);
        this->setItem(row + i, 0, new QTableWidgetItem(text.at(i)));
        this->blockSignals(false);
        emit updateTable(row + i, text.at(i), UiCommandMap::PL_PASTE_TABLE);
    }
}

void PluginCommandTable::pasteEnterAction()
{
    QClipboard *clipboard = QApplication::clipboard();

    QStringList text = clipboard->text().split(QRegularExpression("\\t|\\n|\\r\\n"));

    //last lests unknown ""
    if(text.last() == "") text.removeLast();
    if(text.first() == "") text.removeFirst();

    int txcount = text.count();
    int row = this->currentRow();
    row = (row < 0) ? 0 : row;

    for(int i = 0; i < txcount; i++){
        insertRow(row);
    }

    for(int i = 0; i < txcount; i++){
        this->blockSignals(true);
        this->setItem(row + i, 0, new QTableWidgetItem(text.at(i)));
        this->blockSignals(false);
        emit updateTable(row + i, text.at(i), UiCommandMap::PL_PASTE_TABLE);
    }
}

void PluginCommandTable::editedAction(int row, int column)
{
    qDebug() << "[PluginCommandTable : editedAction]";
    emit updateTable(row, this->item(row, column)->text(), UiCommandMap::PL_EDIT_TABLE);
}
