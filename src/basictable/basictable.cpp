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

#include "basictable.h"


BasicTable::BasicTable(QWidget *parent)
    : BaseTable(parent)
{
    //set header sort settings
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &BasicTable::horizontalHeaderClicked);
}

BasicTable::~BasicTable(){

}

void BasicTable::setPopupActionTop(){
    return;
}

void BasicTable::setPopupActionBottom(){
    return;
}

void BasicTable::setPopupActionDefault(QIcon copy, QIcon up, QIcon down)
{
    //set basic items
    m_copy = contextMenu->addAction(copy, tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_up = contextMenu->addAction(up, tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(down, tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    //connect signals
    connect(m_copy, &QAction::triggered, this, &BasicTable::copyAction);
    connect(m_up, &QAction::triggered, this, &BasicTable::upAction);
    connect(m_down, &QAction::triggered, this, &BasicTable::downAction);
}

bool BasicTable::deleteCheckMessage()
{
    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert"), tr("Do you want to delete selected files ?"), QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

void BasicTable::horizontalHeaderClicked(int)
{
    this->selectAll();
}

void BasicTable::editAction()
{
    return;
}

void BasicTable::addAction()
{
    return;
}

void BasicTable::deleteAction()
{
    return;
}

void BasicTable::copyAction()
{
    if(this->rowCount() == 0) return;
    copyTable(currentRow());
}

void BasicTable::upAction()
{
    if(this->rowCount() == 0) return;
    int row = this->row(this->selectedItems().at(0));

    if(row-1 == -1) return;
    swapTableRow(row,row-1);
    selectRow(row-1);
}

void BasicTable::downAction()
{
    if(this->rowCount() == 0) return;
    int row = this->row(this->selectedItems().at(0));

    if(row+1 == this->rowCount()) return;
    swapTableRow(row,row+1);
    selectRow(row+1);
}
