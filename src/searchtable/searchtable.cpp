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
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // popupAction
    setPopupActionTop();
    setPopupActionDefault(getIcon(ACTION::COPY)
                          , getIcon(ACTION::UP)
                          , getIcon(ACTION::DOWN));
    setPopupActionBottom();

    // init table size
    setColumnCount(3);
    setRowCount(0);

    // adjust row
    resizeRowsToContents();

    // remove row number
    verticalHeader()->setVisible(false);

    // adjust column
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    // set header label
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name")
                                             << tr("Search Word")
                                             << tr("Directory")));

    // set new xml builder
    builder = new SearchXmlBuilder();

    // init table (reload read file.)
    reloadAction();

    // set edit action
    connect(this, &QTableWidget::cellDoubleClicked
            , this, &SearchTable::launchSearchSetting);
}

SearchTable::~SearchTable()
{
    delete builder;
}

void SearchTable::setPopupActionTop()
{
    // set basic items
    m_add = addTableAction(ACTION::ADD, Qt::CTRL, Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL, Qt::Key_E);
    contextMenu->addSeparator();

    // connect signals
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

          default:                                              break;
        }
       return true;
   }

   // standard event processing
   return QObject::eventFilter(obj, event);
}

/**
 * @fn SearchTable::addAction
 * @brief Launch search settings and display summary in table if accepted.
 */
void SearchTable::addAction()
{
    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
    fs->move(this->mapToGlobal(
                 this->geometry().center()) - fs->rect().center());
    fs->setWindowTitle(tr("Editing - Untitled*"));

    if(fs->exec() == QDialog::Accepted){
        int index = this->rowCount();
        setRowCount(index + 1);
        setTableItem(index);
    }
}

/**
 * @fn SearchTable::editAction
 * @brief Launch search settings for the selected location.
 */
void SearchTable::editAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;
    launchSearchSetting(this->currentRow());
}

/**
 * @fn SearchTable::deleteAction
 * @brief Deletes the selected tableItem and xml elements synchronously.
 */
void SearchTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
    {
        deleteTableRecursive([&](int row){
            //delete file item
            builder->deleteItem(row);
        });
    }
}

/**
 * @fn SearchTable::copyAction
 * @brief Duplicate the selected element.
 */
void SearchTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    builder->copyItem(this->currentRow());

    reloadAction();

    selectRow(this-> rowCount() - 1);
}

/**
 * @fn SearchTable::upAction
 * @brief Move up one selection line.
 */
void SearchTable::upAction()
{
    upSelected([&](int current) {
        builder->swapItem(current, current - 1);
        reloadAction();
    });
}

/**
 * @fn SearchTable::downAction
 * @brief Move down one selection line.
 */
void SearchTable::downAction()
{   
    downSelected([&](int current) {
        builder->swapItem(current, current + 1);
        reloadAction();
    });
}

/**
 * @fn SearchTable::reloadAction
 * @brief Update all table items.
 */
void SearchTable::reloadAction()
{
    QList<QList<QStringList> *> item;
    builder->readAll(&item);
    int count = static_cast<int>(item.count());

    // set tables
    setRowCount(count);

    for(int i = 0; i < count; i++){
        setTableItem(i, item.at(i));
    }
}

/**
 * @fn SearchTable::launchSearchSetting
 * @brief
 * Open the search settings for the specified line
 * so that it can be edited.
 * @param row Selected row index.
 * @param col Unused.
 */
void SearchTable::launchSearchSetting(int row, int col)
{
    Q_UNUSED(col)

    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
    fs->move(this->mapToGlobal(
                 this->geometry().center()) - fs->rect().center());

    QList<QStringList> list;
    if(!builder->readItem(row, &list)) return;

    //set title
    fs->loadSettingList(row, &list);

    if(fs->exec() == QDialog::Accepted){
        setTableItem(row);
    }
}

/**
 * @fn SearchTable::setTableItem
 * @brief
 * Set table elements for one row.
 * This function reads the XML of the specified line directly,
 * so it is slow if you call it frequently.
 * @param row Row to set the table.
 */
void SearchTable::setTableItem(int row)
{
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        setTableItem(row, list);
    }
    delete list;
}

/**
 * @fn SearchTable::setTableItem
 * @brief Set table elements for one row.
 * @param row Row to set the table.
 * @param item One line of data to retrieve from XML.
 */
void SearchTable::setTableItem(int row, const QList<QStringList> *item)
{
    //set tableitem
    this->setItem(row
                  , 0
                  , new QTableWidgetItem(xf.fetch(item, SEARCH_NAME)));

    this->setItem(row
                  , 1
                  , new QTableWidgetItem(xf.fetch(item, SEARCH_KEYWORD)));

    this->setItem(row
                  , 2
                  , new QTableWidgetItem(xf.fetch(item, SEARCH_DIR)));
}
