/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "basetable.h"

BaseTable::BaseTable(QWidget *)
{
    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested
            , [=](const QPoint &){ contextMenu->popup(QCursor::pos()); });

    //init menu context
    contextMenu = new QMenu(this);
}

BaseTable::~BaseTable(){
    delete contextMenu;
}

void BaseTable::dropEvent(QDropEvent *event)
{
    return QTableWidget::dropEvent(event);
}

void BaseTable::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

/**
 * @fn BaseTable::eventFilter
 * @brief grab key event
 * @param obj
 * @param event
 * @return
 */
bool BaseTable::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << event->type();
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            qDebug() << "Enter";
            break;
        case Qt::Key_Escape:
            qDebug() << "Escape";
            break;
        case Qt::Key_Insert:
            qDebug() << "Insert";
            break;
        case Qt::Key_Delete:
            qDebug() << "Delete";
            deleteTableRecursive();
            break;
        default:
            qDebug("Ate key press %d", keyEvent->key());
            break;
        }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}


QStringList BaseTable::droppedFromOutside(QDropEvent *event)
{
    QStringList droppeddata;

    for(const QUrl& url: event->mimeData()->urls()){
        droppeddata.append(url.toLocalFile());
    }

    event->acceptProposedAction();
    return droppeddata;
}

/**
 * @fn BaseTable::insideDropRowsMove
 * @brief
 * Move multiple rows when dropped action.
 * Allowed UI selects are single selection and multiple continuous ones.
 *
 * @param event    dropEvent action argument.
 * @param selected if you needed to get a rows list of before moved.
 *
 * @return operation success or falied.
 */
bool BaseTable::insideDropRowsMove(QDropEvent *event, QList<int> *selected)
{   
    // Check dropped row
    int droppedrow = this->indexAt(event->position().toPoint()).row();
    return droppedFromInside(droppedrow, selected);
}

/**
 * @fn BaseTable::droppedFromInside
 * @brief
 * Move multiple rows when dropped action.
 * Allowed UI selects are single selection and multiple continuous ones.
 *
 * @param droppedrow Dropped row index
 * @param selected   if you needed to get a rows list of before moved.
 *
 * @return operation success or falied.
 */
bool BaseTable::droppedFromInside(int droppedrow, QList<int> *selected)
{
    // Check dropped row
    if(droppedrow == -1) return false;

    QModelIndexList mlist = this->selectedIndexes();

    // Sort list ascend order
    std::sort(mlist.begin(), mlist.end());

    if(!checkRowContinuous(&mlist)) return false;

    // Stores the row number and element
    // before the selection value is moved.
    QHash<int, QList<QTableWidgetItem>> selectlist;
    int cols = this->horizontalHeader()->count();

    for (int i = 0; i < mlist.count(); i+=cols) {
        QList<QTableWidgetItem> widget;
        for(int j = 0; j < cols; j++){
            widget.append(*this->item(mlist.at(i).row(), j));
        }
        selectlist.insert(mlist.at(i).row(), widget);
    }

    // Swap multiple lines
    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int updown = 0;
    int before = 0;
    QList<QTableWidgetItem> beforedata;
    int deductnum = 0;

    qsizetype rows = selectlist.count();

    for (int i = 0; i < rows; i++) {

        before = mlist.at(i * cols).row();

        if(selected != nullptr){
            selected->append(before);
        }

        if(before > droppedrow){

            if(!lastelement){
                lastelement = true;
                deleterow = mlist.last().row();
            }

            beforedata = selectlist.value(deleterow - deductnum);
            deductnum++;
            updown = 0;

        }else{

            if(!firstelement){
                firstelement = true;
                deleterow = mlist.first().row();
            }

            beforedata = selectlist.value(before);
            updown = -1;
        }

        this->blockSignals(true);
        this->removeRow(deleterow);
        this->insertRow(droppedrow + updown);
        for(int j = 0; j < cols; j++){
            this->setItem(droppedrow + updown
                          , j
                          , new QTableWidgetItem(beforedata.at(j)));
        }
        this->blockSignals(false);
    }

    qDebug() << "[BaseTable::droppedFromInside] droppedrow : " << droppedrow;

    return true;
}

/**
 * @fn BaseTable::swapTableRow
 * @brief Swapping elements within two indexes.
 *
 * @param from The row index of the move source.
 * @param dest Destination row index.
 */
void BaseTable::swapTableRow(int from, int dest)
{
    QTableWidgetItem *temp;
    int columncount = this->columnCount();

    for (int col = 0; col < columncount; col++){
        temp = this->takeItem(from, col);
        this->setItem(from, col, this->takeItem(dest, col));
        this->setItem(dest, col, temp);
    }
}

/**
 * @fn BaseTable::deleteTableRecursive
 * @brief Delete user-selected table items in UI.
 */
void BaseTable::deleteTableRecursive()
{
    deleteTableRecursive([](int dummy){Q_UNUSED(dummy)});
}

/**
 * @fn BaseTable::deleteTableRecursive
 * @brief Delete user-selected table items in UI.
 *
 * @param predelete
 * Another process to do before deleting a table.
 * The argument is the row number of the table to be deleted.
 */
void BaseTable::deleteTableRecursive(std::function<void (int)> predelete)
{
    QModelIndexList lists = this->selectionModel()->selectedRows();

    while(!lists.empty()){
        predelete(lists.last().row());
        this->removeRow(lists.last().row());
        lists.removeLast();
    }
}

/**
 * @fn BaseTable::checkRowContinuous
 * @brief Check if row selection is continuous.
 *
 * @param indexes : List to judge.
 *
 * @return True if continuous, false if discontinuous.
 */
bool BaseTable::checkRowContinuous(QModelIndexList *indexes)
{
    // Check row selections are of continuous value.
    if(indexes->count() > 1){
        int col = this->horizontalHeader()->count();

        for (int i = 1; i < indexes->count(); i+=col) {
            int deduct = indexes->at(i).row() - indexes->at(i-1).row();
            if(deduct > 2 || deduct < 0){
                return false;
            }
        }
    }

    return true;
}
