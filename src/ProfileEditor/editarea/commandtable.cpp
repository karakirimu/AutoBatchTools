﻿/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "commandtable.h"

CommandTable::CommandTable(QWidget *parent)
    : BasicTable(parent)
{
    // popupAction
    setPopupActionTop();
    setPopupActionDefault();

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    // init table size
    setColumnCount(1);
    setRowCount(0);

    // adjust row
    resizeRowsToContents();

    // adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // set vertical header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                    , qpc.VERTICAL_HEADER_ENABLE);

    // set header label
    setHorizontalHeaderLabels((QStringList()
                               << tr("Executable file / Arguments")));

    connect(this,&CommandTable::cellChanged
            , this, &CommandTable::editedAction);
}

CommandTable::~CommandTable()
{

}

QString CommandTable::getText(int row)
{
    return this->model()->index(row, 0).data().toString();
}

void CommandTable::insertItem(int row)
{
    this->blockSignals(true);
    this->clearSelection();
    this->insertRow(row);
    this->selectRow(row);
    this->blockSignals(false);
}

void CommandTable::deleteItem(int row)
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;
    this->removeRow(row);
    this->clearSelection();
}

void CommandTable::replaceItem(int row, QString str)
{
    this->blockSignals(true);
    this->setItem(row, 0, new QTableWidgetItem(str));
    this->clearSelection();
    selectRow(row);
    this->blockSignals(false);
}

void CommandTable::swapItem(int before, int after)
{
    //swap item
    QString tmp = this->model()->index(before, 0).data().toString();
    this->blockSignals(true);
    this->setItem(before
                  , 0
                  , new QTableWidgetItem(this
                                           ->model()
                                           ->index(after, 0)
                                           .data()
                                           .toString()));
    this->setItem(after
                  , 0
                  , new QTableWidgetItem(tmp));
    this->blockSignals(false);

    this->clearSelection();
    selectRow(after);
}

/**
 * @fn CommandTable::moveItem
 * @brief moving item in table (single or multiple contiguous select only)
 * @param before The first-line number to move from.
 * @param beforecount The number of selections from the 'before' param.
 * @param after The first line number to move.
 */
void CommandTable::moveItem(int before, int beforecount, int after)
{
    QHash<int, QString> selectlist;

    if(before < after){
        for (int i = 0; i < beforecount; i++) {
            selectlist.insert(before + i
                              , this->model()->index(before + i, 0)
                                  .data()
                                  .toString());
        }

    }else{
        int bc = before - beforecount + 1;
        for (int i = 0; i < beforecount; i++) {
            selectlist.insert(bc + i
                              , this->model()->index(bc + i, 0)
                                  .data()
                                  .toString());
        }
    }

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

//    int updown = 0;
    QString beforedata;
    int deductnum = 0;

    for (int i = 0; i < beforecount; i++) {

        if(before > after){

            if(!lastelement){
                lastelement = true;
                deleterow = before;
            }

            beforedata = selectlist.value(before - deductnum);
            deductnum++;
//            updown = 0;

        }else{

            if(!firstelement){
                firstelement = true;
                deleterow = before;
            }

            beforedata = selectlist.value(before + i);
//            updown = -1;
        }

        this->blockSignals(true);
        this->removeRow(deleterow);
        this->insertRow(after /*+ updown*/);
        this->setItem(after /*+ updown*/, 0, new QTableWidgetItem(beforedata));
        this->blockSignals(false);
    }
}

void CommandTable::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "[CommandTable::dragEnterEvent] Object : "
             << event->source()->objectName();
    if(event->source() != nullptr){
        event->acceptProposedAction();
    }
}

void CommandTable::dropEvent(QDropEvent *event)
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    int droppedrow = this->indexAt(event->position().toPoint()).row();

    QList<int> beforeindex;
    if(!BaseTable::droppedFromInside(droppedrow, &beforeindex)) return;

    qDebug() << "[CommandTable::dropEvent] droppedrow : " << droppedrow;

    emit dragDropTable(beforeindex, droppedrow);
}

void CommandTable::addAction()
{
    int row = this->rowCount();
    this->insertRow(row);
    emit updateTable(row, "", UiCommandMap::E_ADD_TABLE);

    //for useability
    this->setCurrentItem(itemAt(row,0));
    this->selectRow(row);
}

void CommandTable::editAction()
{
    this->clearSelection();
    this->edit(currentIndex());
}

/**
 * @fn CommandTable::deleteAction
 * @brief delete table item(s)
 */
void CommandTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(!deleteCheckMessage()) return;

//    QModelIndexList lists = this->selectedIndexes();

//    while(!lists.empty()){
//        this->removeRow(lists.at(0).row());
//        emit updateTable(lists.at(0).row(), "", UiCommandMap::E_DELETE_TABLE);

//        lists = this->selectedIndexes();
//    }

    deleteTableRecursive([&](int row){
        emit updateTable(row, "", UiCommandMap::E_DELETE_TABLE);
    });
}

/**
 * @fn CommandTable::cutAction
 * @brief cut selected command
 * @note multiple select (it msy be not so good ...)
 */
void CommandTable::cutAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();
    qsizetype rows = mlist.count();
    for(int i = 0; i < rows; i++){
        tmp.append(mlist.at(i).data().toString());
        if(i < rows) tmp.append("\t");

        emit updateTable(mlist.at(i).row(), "", UiCommandMap::E_CUT_TABLE);
    }

    //force delete
    BaseTable::deleteTableRecursive();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);
}

/**
 * @fn CommandTable::copyAction
 * @brief copy selected commands to clipboard
 */
void CommandTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();
    qsizetype rows = mlist.count();
    for(int i = 0; i < rows; i++){
        tmp.append(mlist.at(i).data().toString());
        if(i < rows) tmp.append("\t");
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);

}

/**
 * @fn CommandTable::pasteAction
 * @brief paste commands from clipboard
 */
void CommandTable::pasteAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split("\t");

    //last lests unknown ""
    if(text.last() == "") text.removeLast();

    int row = this->rowCount();
    qsizetype txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);
       this->blockSignals(true);
       this->setItem(row, 0, new QTableWidgetItem(text.at(i)));
       this->blockSignals(false);
       emit updateTable(row, text.at(i), UiCommandMap::E_PASTE_TABLE);
    }
}

void CommandTable::pasteSpaceAction()
{
    paste(QRegularExpression("\\t| "));
}

void CommandTable::pasteEnterAction()
{
    paste(QRegularExpression("\\t|\\n|\\r\\n"));
}

/**
 * @fn CommandTable::upAction
 * @brief Swap selected row with the row above
 */
void CommandTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;
    this->swapItem(current, current - 1);

    emit swapTable(current, current - 1);
}

/**
 * @fn CommandTable::downAction
 * @brief Swap selected row with the row below
 */
void CommandTable::downAction()
{
    int current = this->currentRow();
    if((current + 1) == this->rowCount()) return;
    this->swapItem(current, current + 1);

    emit swapTable(current, current + 1);
}

void CommandTable::openFileAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFile(QDir::currentPath());
    if(str != ""){
        this->setItem(current, 0, new QTableWidgetItem(str));
        editedAction(current, 0);
    }
}

void CommandTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder(QDir::currentPath());
    if(str != ""){
        this->setItem(current, 0, new QTableWidgetItem(str));
        editedAction(current, 0);
    }
}

void CommandTable::editedAction(int row, int column)
{
    qDebug() << "[CommandTable : editedAction] row:" << row
             << ", column:" << column;
    QString text = this->item(row, column)->text();
    emit updateTable(row, text, UiCommandMap::E_EDIT_TABLE);
}

/**
 * @fn CommandTable::setPopupActionTop
 * @brief popup action menu definition
 */
void CommandTable::setPopupActionTop()
{
    //set basic items
    m_add = addTableAction(ACTION::ADD, Qt::ALT, Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::ALT, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::ALT, Qt::Key_E);
    contextMenu->addSeparator();

    m_file = addTableAction(ACTION::FILE);
    m_dir = addTableAction(ACTION::FOLDER);
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &CommandTable::addAction);
    connect(m_delete, &QAction::triggered, this, &CommandTable::deleteAction);
    connect(m_edit, &QAction::triggered, this, &CommandTable::editAction);

    connect(m_file, &QAction::triggered, this, &CommandTable::openFileAction);
    connect(m_dir, &QAction::triggered, this, &CommandTable::openDirectoryAction);
}

void CommandTable::setPopupActionDefault()
{
    //set basic items
    m_cut = addTableAction(ACTION::CUT, Qt::ALT, Qt::Key_X);
    m_copy = addTableAction(ACTION::COPY, Qt::ALT, Qt::Key_C);

    contextMenu->addSeparator();

    m_paste = addTableAction(ACTION::PASTE, Qt::ALT, Qt::Key_V);

    m_pastespace = addTableAction(ACTION::PASTESPACE);
    m_pasteenter = addTableAction(ACTION::PASTENEWLINE);

    contextMenu->addSeparator();

    m_up = addTableAction(ACTION::UP, Qt::ALT, Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::ALT, Qt::Key_Down);

    //connect signals
    connect(m_cut, &QAction::triggered, this, &CommandTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &CommandTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &CommandTable::pasteAction);
    connect(m_pastespace, &QAction::triggered, this, &CommandTable::pasteSpaceAction);
    connect(m_pasteenter, &QAction::triggered, this, &CommandTable::pasteEnterAction);
    connect(m_up, &QAction::triggered, this, &CommandTable::upAction);
    connect(m_down, &QAction::triggered, this, &CommandTable::downAction);
}

void CommandTable::paste(const QRegularExpression &exp)
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(exp);

    //last lests unknown ""
    if(text.last() == "") text.removeLast();
    if(text.first() == "") text.removeFirst();

    qsizetype txcount = text.count();
    int row = this->currentRow();
    row = (row < 0) ? 0 : row;

    for(int i = 0; i < txcount; i++){
        insertRow(row);
    }

    for(int i = 0; i < txcount; i++){
        this->blockSignals(true);
        this->setItem(row + i, 0, new QTableWidgetItem(text.at(i)));
        this->blockSignals(false);
        emit updateTable(row + i, text.at(i), UiCommandMap::E_PASTE_TABLE);
    }
}

bool CommandTable::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

    auto altCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::AltModifier);
    };

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:  if (altCheck()) addAction();    break;
           case Qt::Key_Delete: if (altCheck()) deleteAction(); break;
           case Qt::Key_E:      if (altCheck()) editAction();   break;

           case Qt::Key_Up:
             if(altCheck()){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             }
             break;

           case Qt::Key_Down:
             if(altCheck()){
                 downAction();
             }else{
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
             }
            break;

           case Qt::Key_X:  if(altCheck()) cutAction();    break;
           case Qt::Key_C:  if(altCheck()) copyAction();   break;
           case Qt::Key_V:  if(altCheck()) pasteAction();  break;
           default:
             //qDebug("Ate key press %d", keyEvent->key());
             break;
         }
        return true;
    }

//    qDebug() << "[CommandTable::eventFilter] Event type : " << event->type();
//    qDebug() << "[CommandTable::eventFilter]" << event;
    // standard event processing
    return QObject::eventFilter(obj, event);
}
