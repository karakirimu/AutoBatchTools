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
        this
        , tr("Alert")
        , tr("Do you want to delete selected item(s)?")
        , QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

/**
 * @fn BasicTable::addTableAction
 *
 * @brief A wrapper function to simplify action creation.
 *        The icon path and the corresponding text are all
 *        defined in the class.
 *
 * @param action "enum ACTION" defined in this class
 * @param keycode QKeySequence keycode
 *
 * @return created action
 */
QAction *BasicTable::addTableAction(ACTION action, int keycode)
{
    QAction *res = contextMenu->addAction(getIcon(action), getActionText(action));
    if(keycode != 0){
        res->setShortcutVisibleInContextMenu(true);
        res->setShortcut(QKeySequence(keycode));
    }
    return res;
}

/**
 * @fn BasicTable::getIcon
 *
 * @brief Default icon path definition
 *
 * @param action "enum ACTION" defined in this class
 *
 * @return QIcon with path assigned
 */
QIcon BasicTable::getIcon(ACTION action)
{
    QIcon icon;
    QString basename = ":/default_icons/";

    switch (action) {
    case ACTION::NEWFILE:       icon.addFile(basename + "newfile.png");    break;
    case ACTION::ADD:           icon.addFile(basename + "add.png");        break;
    case ACTION::REMOVE:        icon.addFile(basename + "remove.png");     break;
    case ACTION::EDIT:          icon.addFile(basename + "edit.png");       break;
    case ACTION::ENABLE:        icon.addFile(basename + "enable.png");     break;
    case ACTION::DISABLE:       icon.addFile(basename + "error.png");      break;
    case ACTION::CLEAR:         icon.addFile(basename + "blank.png");      break;
    case ACTION::FILE:
    case ACTION::FILES:         icon.addFile(basename + "file.png");       break;
    case ACTION::FOLDER:        icon.addFile(basename + "folder.png");     break;
    case ACTION::FILEINFO:      icon.addFile(basename + "info.png");       break;
    case ACTION::CUT:           icon.addFile(basename + "cut.png");        break;
    case ACTION::COPY:          icon.addFile(basename + "copy.png");       break;
    case ACTION::PASTE:         icon.addFile(basename + "paste.png");      break;
    case ACTION::PASTESPACE:    icon.addFile(basename + "blank.png");      break;
    case ACTION::PASTENEWLINE:  icon.addFile(basename + "blank.png");      break;
    case ACTION::UP:            icon.addFile(basename + "arrow_up.png");   break;
    case ACTION::DOWN:          icon.addFile(basename + "arrow_down.png"); break;
    case ACTION::REFRESH:       icon.addFile(basename + "refresh.png");    break;
    case ACTION::PROPERTY:      icon.addFile(basename + "blank.png");      break;
    }


    return icon;
}

/**
 * @fn BasicTable::getActionText
 *
 * @brief Define default action text
 *
 * @param action "enum ACTION" defined in this class
 *
 * @return The text corresponding to the action
 */
QString BasicTable::getActionText(ACTION action)
{
    QString text = "undefined";

    switch (action) {
    case ACTION::NEWFILE:       text = tr("New");                       break;
    case ACTION::ADD:           text = tr("Add");                       break;
    case ACTION::REMOVE:        text = tr("Delete");                    break;
    case ACTION::EDIT:          text = tr("Edit");                      break;
    case ACTION::ENABLE:        text = tr("Enable");                    break;
    case ACTION::DISABLE:       text = tr("Disable");                   break;
    case ACTION::CLEAR:         text = tr("Clear");                     break;
    case ACTION::FILE:          text = tr("Add File");                  break;
    case ACTION::FILES:         text = tr("Add Files");                 break;
    case ACTION::FOLDER:        text = tr("Add Folder");                break;
    case ACTION::FILEINFO:      text = tr("File Info");                 break;
    case ACTION::CUT:           text = tr("Cut");                       break;
    case ACTION::COPY:          text = tr("Copy");                      break;
    case ACTION::PASTE:         text = tr("Paste");                     break;
    case ACTION::PASTESPACE:    text = tr("Paste (space separated)");   break;
    case ACTION::PASTENEWLINE:  text = tr("Paste (newline separated)"); break;
    case ACTION::UP:            text = tr("Up");                        break;
    case ACTION::DOWN:          text = tr("Down");                      break;
    case ACTION::REFRESH:       text = tr("Reload");                    break;
    case ACTION::PROPERTY:      text = tr("Property");                  break;
    }

    return text;
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
