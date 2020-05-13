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

#include "commandtable.h"

CommandTable::CommandTable(QWidget *parent)
    : BasicTable(parent)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault();

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set vertical header style
    verticalHeader()->setProperty("VerticalHeaderStyle", 1);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Executable file / Arguments")));

    connect(this,&CommandTable::cellChanged, this, &CommandTable::editedAction);
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
    this->setItem(before, 0, new QTableWidgetItem(this->model()->index(after, 0).data().toString()));
    this->setItem(after, 0, new QTableWidgetItem(tmp));
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
            selectlist.insert(before + i, this->model()->index(before + i, 0).data().toString());
        }

    }else{
        int bc = before - beforecount + 1;
        for (int i = 0; i < beforecount; i++) {
            selectlist.insert(bc + i, this->model()->index(bc + i, 0).data().toString());
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
    qDebug() << "[CommandTable::dragEnterEvent] Object : " << event->source()->objectName();
    if(event->source() != nullptr){
        event->acceptProposedAction();
    }
}

void CommandTable::dropEvent(QDropEvent *event)
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    int droppedrow = this->indexAt(event->pos()).row();

    QList<int> beforeindex;
    if(!BaseTable::insideDropRowsMove(event, &beforeindex)) return;

    qDebug() << "[CommandTable::dropEvent] droppedrow : " << droppedrow;

    emit dragDropTable(beforeindex, droppedrow);
}

void CommandTable::addAction()
{
    int row = this->rowCount();
    this->insertRow(row);
    emit updateTable(row, "", ProcessXmlListGenerator::TABLE_ADD);

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

    QModelIndexList lists = this->selectedIndexes();

    while(!lists.empty()){
        this->removeRow(lists.at(0).row());
        emit updateTable(lists.at(0).row(), "", ProcessXmlListGenerator::TABLE_DELETE);

        lists = this->selectedIndexes();
    }
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
    int rows = mlist.count();
    for(int i = 0; i < rows; i++){
        tmp.append(mlist.at(i).data().toString());
        if(i < rows) tmp.append("\t");

        emit updateTable(mlist.at(i).row(), "", ProcessXmlListGenerator::TABLE_CUT);
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
    int rows = mlist.count();
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
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);
       this->blockSignals(true);
       this->setItem(row, 0, new QTableWidgetItem(text.at(i)));
       this->blockSignals(false);
       emit updateTable(row, text.at(i), ProcessXmlListGenerator::TABLE_PASTE);
    }
}

void CommandTable::pasteSpaceAction()
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
       emit updateTable(row + i, text.at(i), ProcessXmlListGenerator::TABLE_PASTE);
    }

}

void CommandTable::pasteEnterAction()
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
       emit updateTable(row + i, text.at(i), ProcessXmlListGenerator::TABLE_PASTE);
    }
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
    //this->in
    this->setItem(current, 0, new QTableWidgetItem(str));
    if(str != ""){
        editedAction(current, 0);
    }
}

void CommandTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder(QDir::currentPath());
    this->setItem(current, 0, new QTableWidgetItem(str));
    if(str != ""){
        editedAction(current, 0);
    }
}

void CommandTable::editedAction(int row, int column)
{
    qDebug() << "[CommandTable : editedAction]";
     emit updateTable(row, this->item(row, column)->text(), ProcessXmlListGenerator::TABLE_EDIT);
}

/**
 * @fn CommandTable::setPopupActionTop
 * @brief popup action menu definition
 */
void CommandTable::setPopupActionTop()
{
    //set basic items
    m_add = addTableAction(ACTION::ADD, Qt::ALT + Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::ALT + Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::ALT + Qt::Key_E);
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
    m_cut = addTableAction(ACTION::CUT, Qt::ALT + Qt::Key_X);
    m_copy = addTableAction(ACTION::COPY, Qt::ALT + Qt::Key_C);

    contextMenu->addSeparator();

    m_paste = addTableAction(ACTION::PASTE, Qt::ALT + Qt::Key_V);

    m_pastespace = addTableAction(ACTION::PASTESPACE);
    m_pasteenter = addTableAction(ACTION::PASTENEWLINE);

    contextMenu->addSeparator();

    m_up = addTableAction(ACTION::UP, Qt::ALT + Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::ALT + Qt::Key_Down);

    //connect signals
    connect(m_cut, &QAction::triggered, this, &CommandTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &CommandTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &CommandTable::pasteAction);
    connect(m_pastespace, &QAction::triggered, this, &CommandTable::pasteSpaceAction);
    connect(m_pasteenter, &QAction::triggered, this, &CommandTable::pasteEnterAction);
    connect(m_up, &QAction::triggered, this, &CommandTable::upAction);
    connect(m_down, &QAction::triggered, this, &CommandTable::downAction);
}

bool CommandTable::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

    auto mdCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::AltModifier);
    };

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:  if (mdCheck()) addAction();    break;
           case Qt::Key_Delete: if (mdCheck()) deleteAction(); break;
           case Qt::Key_E:      if (mdCheck()) editAction();   break;

           case Qt::Key_Up:
             if(mdCheck()){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             }
             break;

           case Qt::Key_Down:
             if(mdCheck()){
                 downAction();
             }else{
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
             }
            break;

           case Qt::Key_X:  if(mdCheck()) cutAction();    break;
           case Qt::Key_C:  if(mdCheck()) copyAction();   break;
           case Qt::Key_V:  if(mdCheck()) pasteAction();  break;

           default:
             //qDebug("Ate key press %d", keyEvent->key());
             break;
         }
        return true;
    }

//    qDebug() << "[CommandTable::eventFilter] Event type : " << event->type();

    // standard event processing
    return QObject::eventFilter(obj, event);
}
