/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "searchtable.h"

SearchTable::SearchTable(QWidget *)
{
    //disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault(getIcon(ACTION::COPY), \
                          getIcon(ACTION::UP), \
                          getIcon(ACTION::DOWN));
    setPopupActionBottom();

    //init table size
    setColumnCount(3);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //remove row number
    verticalHeader()->setVisible(false);

    //adjust column
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name")
                               << tr("Search Word") << tr("Directory")));

    //set new xml builder
    builder = new SearchXmlBuilder();

    //init table (reload read file.)
    reloadAction();

    //set doubleclick action
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editAction()));
}

SearchTable::~SearchTable()
{
    delete builder;
}

void SearchTable::setPopupActionTop()
{
    //set basic items
    m_add = addTableAction(ACTION::ADD, Qt::CTRL, Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL, Qt::Key_E);
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &SearchTable::addAction);
    connect(m_delete, &QAction::triggered, this, &SearchTable::deleteAction);
    connect(m_edit, &QAction::triggered, this, &SearchTable::editAction);
}

void SearchTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL, Qt::Key_R);

    connect(m_ref, &QAction::triggered, this, &SearchTable::reloadAction);
}

bool SearchTable::eventFilter(QObject *obj, QEvent *event)
{
   QKeyEvent *keyEvent;

   auto mdCheck = [&keyEvent](){
       return static_cast<bool>(keyEvent->modifiers() & Qt::ControlModifier);
   };

   //qDebug() << event->type();
   if (event->type() == QEvent::KeyPress) {
       keyEvent = static_cast<QKeyEvent *>(event);
       switch (keyEvent->key())
        {
          case Qt::Key_Return:
          case Qt::Key_Enter:  if (mdCheck())  addAction();     break;
          case Qt::Key_Delete:                 deleteAction();  break;

          case Qt::Key_Up:
            if (mdCheck()){
                upAction();
            }else{
                if(this->currentRow() != 0)
                    selectRow(this->currentRow() - 1);
            }
            break;

          case Qt::Key_Down:
            if (mdCheck()){
                downAction();
            }else{
                if(this->rowCount() - 1 != this->currentRow())
                    selectRow(this->currentRow() + 1);
            }
           break;

          case Qt::Key_C:      if (mdCheck())  copyAction();    break;
          case Qt::Key_E:      if (mdCheck())  editAction();    break;
          case Qt::Key_R:      if (mdCheck())  reloadAction();  break;

          default:
            //qDebug("Ate key press %d", keyEvent->key());
            break;
        }
       return true;
   }
   // standard event processing
   return QObject::eventFilter(obj, event);
}

void SearchTable::addAction()
{
    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
    fs->move(this->mapToGlobal(this->geometry().center()) - fs->rect().center());
    fs->setWindowTitle(tr("Editing - Untitled*"));
    if(fs->exec() == QDialog::Accepted){
        int index = this->rowCount();
        setRowCount(index + 1);
        setTableItem(index);
    }
}

void SearchTable::editAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
    fs->move(this->mapToGlobal(this->geometry().center()) - fs->rect().center());
    QList<QStringList> list;
    int row = currentRow();
    if(builder->readItem(row, &list)){
        //set title
        fs->loadSettingList(row, &list);
    }
    if(fs->exec() == QDialog::Accepted){
        setTableItem(row);
    }
}

void SearchTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
    {
        //delete file item
        builder->deleteItem(currentRow());

        //reload
        reloadAction();
    }
}

void SearchTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    builder->copyItem(this->currentRow());

    reloadAction();

    selectRow(this-> rowCount() - 1);
}

void SearchTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    builder->swapItem(current, current-1);

    reloadAction();

    selectRow(current - 1);
}

void SearchTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    builder->swapItem(current, current+1);

    reloadAction();

    selectRow(current + 1);
}

void SearchTable::reloadAction()
{
    int count = builder->count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i);
    }
}

void SearchTable::setTableItem(int row)
{
    //qDebug () << "setTableItem";
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        //set tableitem
        this->setItem(row,0,new QTableWidgetItem(builder->fetch(SEARCH_NAME,SEARCH_NONE, list)));
        this->setItem(row,1,new QTableWidgetItem(builder->fetch(SEARCH_KEYWORD,SEARCH_NONE, list)));
        this->setItem(row,2,new QTableWidgetItem(builder->fetch(SEARCH_DIR,SEARCH_NONE, list)));
    }
    delete list;
}
