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

#include "basetable.h"

BaseTable::BaseTable(QWidget *)
{
    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &BaseTable::onCustomContextMenu);

    //init menu context
    contextMenu = new QMenu(this);
}

BaseTable::~BaseTable(){
    delete contextMenu;
}

void BaseTable::dropEvent(QDropEvent *)
{
    return;
}

void BaseTable::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
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

void BaseTable::insideDropRowMove(QDropEvent *event)
{
    insertTableRow(this->row(this->selectedItems().at(0))
                       ,this->indexAt(event->position().toPoint()).row());
}

/**
 * @fn BaseTable::insideDropRowsMove
 * @brief
 * Move multiple rows when dropped action.
 * Allowed UI selects are single selection and multiple continuous ones.
 *
 * @param event    : dropEvent action argument.
 * @param selected : if you needed to get a rows list of before moved.
 *
 * @return operation success or falied.
 */
bool BaseTable::insideDropRowsMove(QDropEvent *event, QList<int> *selected)
{   
    // Check dropped row
    int droppedrow = this->indexAt(event->position().toPoint()).row();
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
            this->setItem(droppedrow + updown, j, new QTableWidgetItem(beforedata.at(j)));
        }
        this->blockSignals(false);
    }

    qDebug() << "[BaseTable::insideDropRowsMove] droppedrow : " << droppedrow;

    return true;
}

void BaseTable::swapTableRow(int from, int dest)
{
    QTableWidgetItem *temp;
    for (int col = 0; col < this->columnCount(); col++){
        temp = this->takeItem(from, col);
        this->setItem(from, col, this->takeItem(dest, col));
        this->setItem(dest, col, temp);
    }
}

void BaseTable::insertTableRow(int from, int dest)
{
    int distance = dest - from;
    QTableWidgetItem *temp;

    this->insertRow(dest);

    // + 1 means effect of insertrow
    from = from + ((distance > 0)? 0 : 1);

    for (int col = 0; col < this->columnCount(); col++){
        temp = this->takeItem(from, col);
        this->setItem(dest, col, temp);
    }

    this->removeRow(from);
}

void BaseTable::copyTable(int index){

    this->setRowCount(this->rowCount() + 1);

    for (int col = 0; col < this->columnCount(); col++){

        for(int i = this->rowCount(); i > index; i--){
            //copy action
            this->setItem(i, col, this->takeItem(i-1, col));
        }
    }
}

/**
 * @fn BaseTable::deleteTableRecursive
 * @brief Delete user-selected table items in UI.
 */
void BaseTable::deleteTableRecursive()
{
    //すべての条件で消える
    QModelIndexList lists = this->selectedIndexes();

    while(!lists.empty()){
        this->removeRow(lists.at(0).row());

        //delete column index
        for(int i = 0; i < lists.at(0).column(); i++){
            lists.removeAt(0);
        }
        lists = this->selectedIndexes();
    }
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

/**
 * @fn BaseTable::checkRowContinuous
 * @brief Check if row selection is continuous.
 * @param indexes : List to judge.
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

void BaseTable::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

QString BaseTable::selectFile(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    return dialog.getOpenFileName(this, tr("Open File"), basedir, tr("File (*.*)"));
}

QString BaseTable::selectFolder(QString basedir){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    return dialog.getExistingDirectory(this, tr("Open Folder"), basedir);
}
