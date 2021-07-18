/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringtable.h"

StringTable::StringTable(QWidget *parent)
    : BasicTable(parent)
{
    // popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    // init table size
    setColumnCount(2);
    setRowCount(0);

    // adjust row
    resizeRowsToContents();

    // adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    // set header label
    setHorizontalHeaderLabels((QStringList()
                               << tr("Variable")
                               << tr("Value")));

    // set vertical header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                    , qpc.VERTICAL_HEADER_ENABLE);

    // set new xml builder
    builder = new StringXmlBuilder();

    // init table (reload read file.)
    reloadAction();

    // set edit saving
    connect(this, &StringTable::cellChanged
            , this, &StringTable::saveCellChanged);

    // install clipboard
    installClipboardFilter(this);
}

StringTable::~StringTable()
{
    delete builder;
}

void StringTable::setPopupActionTop()
{
    //set basic items
    m_add = addTableAction(ACTION::ADD, Qt::CTRL, Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL, Qt::Key_E);
    contextMenu->addSeparator();

    m_file = addTableAction(ACTION::FILE);
    m_dir = addTableAction(ACTION::FOLDER);
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &StringTable::addAction);
    connect(m_delete, &QAction::triggered, this, &StringTable::deleteAction);
    connect(m_edit, &QAction::triggered, this, &StringTable::editAction);
    connect(m_file, &QAction::triggered, this, &StringTable::openFileAction);
    connect(m_dir, &QAction::triggered, this, &StringTable::openDirectoryAction);
}

void StringTable::setPopupActionDefault()
{
    //set basic items
    m_cut = addTableAction(ACTION::CUT, Qt::CTRL, Qt::Key_X);
    m_copy = addTableAction(ACTION::COPY, Qt::CTRL, Qt::Key_C);
    m_paste = addTableAction(ACTION::PASTE, Qt::CTRL, Qt::Key_V);

    contextMenu->addSeparator();

    m_up = addTableAction(ACTION::UP, Qt::CTRL, Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL, Qt::Key_Down);

    //connect signals
    connect(m_cut, &QAction::triggered, this, &StringTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &StringTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &StringTable::pasteAction);
    connect(m_up, &QAction::triggered, this, &StringTable::upAction);
    connect(m_down, &QAction::triggered, this, &StringTable::downAction);
}

void StringTable::setPopupActionBottom()
{
    contextMenu->addSeparator();

    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL, Qt::Key_R);

    connect(m_ref, &QAction::triggered, this, &StringTable::reloadAction);
}

bool StringTable::eventFilter(QObject *obj, QEvent *event)
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

           case Qt::Key_X:      if (mdCheck())  cutAction();     break;
           case Qt::Key_C:      if (mdCheck())  copyAction();    break;
           case Qt::Key_V:      if (mdCheck())  pasteAction();   break;
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

/**
 * @fn StringTable::addAction
 * @brief Add a table and xml elements containing an empty string.
 */
void StringTable::addAction()
{
    int row = this->rowCount();
    setRowCount(row + 1);

    //add empty item
    QList<QStringList> list;
    createList(row, &list);
    builder->addItem(&list);
}

/**
 * @fn StringTable::editAction
 * @brief Enable cursor input for the selected location.
 */
void StringTable::editAction()
{
    this->edit(currentIndex());
}

/**
 * @fn StringTable::deleteAction
 * @brief Deletes the selected tableItem and xml elements synchronously.
 */
void StringTable::deleteAction()
{
    // if rowcount is zero.
    if(this->rowCount() == 0) return;

    // check delete warning message
    if(deleteCheckMessage()) {
        deleteTableRecursive();
        updateXml();
    }
}

/**
 * @fn StringTable::cutAction
 * @brief
 * Deletes the selected tableItem and xml elements synchronously
 * and saves them to the clipboard.
 */
void StringTable::cutAction()
{
    // if rowcount is zero.
    if(this->rowCount() == 0) return;

    copyAction();
    deleteTableRecursive();
    updateXml();
}

/**
 * @fn StringTable::copyAction
 * @brief Copy the selected row to clipboard.
 */
void StringTable::copyAction()
{
    // if rowcount is zero.
    if(this->rowCount() == 0) return;
    copyToClipboard(QApplication::clipboard());
}

/**
 * @fn StringTable::pasteAction
 * @brief
 * Pastes the line copied to the clipboard one line above the selected line.
 */
void StringTable::pasteAction()
{
    if(pasteFromClipboard(QApplication::clipboard())){
        updateXml();
    }
}

/**
 * @fn StringTable::upAction
 * @brief Move up one selection line.
 */
void StringTable::upAction()
{
    upSelected([&](int current) {
        Q_UNUSED(current)
        updateXml();
    });
}

/**
 * @fn StringTable::downAction
 * @brief Move down one selection line.
 */
void StringTable::downAction()
{
    downSelected([&](int current) {
        Q_UNUSED(current)
        updateXml();
    });
}

/**
 * @fn StringTable::openFileAction
 * @brief
 * Select a file and paste the selected
 * file path into the selected row.
 */
void StringTable::openFileAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFile(QDir::currentPath());
    this->setItem(current, 1, new QTableWidgetItem(str));
}

/**
 * @fn StringTable::openDirectoryAction
 * @brief
 * Select a directory and paste the selected
 * folder path into the selected row.
 */
void StringTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder(QDir::currentPath());
    this->setItem(current, 1, new QTableWidgetItem(str));
}

/**
 * @fn StringTable::reloadAction
 * @brief Update all table items.
 */
void StringTable::reloadAction()
{
    QList<QList<QStringList> *> item;
    builder->readAll(&item);
    int count = static_cast<int>(item.count());

    //set tables
    setRowCount(count);

    for(int i = 0; i < count; i++){
        setTableItem(i, item.at(i));
    }
}

/**
 * @fn StringTable::updateXml
 * @brief It takes the data in the table and overwrites the XML file.
 */
void StringTable::updateXml()
{
    int datacount = this->rowCount();
    QList<QList<QStringList> *> itemlist;

    for(int i = 0; i < datacount; i++){
        // add empty item
        QList<QStringList> *list = new QList<QStringList>();
        createList(i, list);
        itemlist.append(list);
    }

    builder->writeAll(&itemlist);
}

/**
 * @fn StringTable::setTableItem
 * @brief Set data for a single row.
 * @param row Row to set the table.
 * @param item One line of data to retrieve from XML.
 */
void StringTable::setTableItem(int row, const QList<QStringList> *item)
{
    // set tableitem
    this->setItem(row,0,new QTableWidgetItem(item->at(0).at(1)));
    this->setItem(row,1,new QTableWidgetItem(item->at(1).at(1)));
}

void StringTable::saveCellChanged(int row, int col)
{
    Q_UNUSED(col)
    //set item
    QList<QStringList> list;
    createList(row, &list);
    builder->editItem(row, &list);
}

/**
 * @fn StringTable::createList
 * @brief Generate data array to be stored in XML from table view.
 * @param row Selected row number
 * @param newlist Result data
 */
void StringTable::createList(int row, QList<QStringList> *newlist)
{
    int cols = this->columnCount();
    QStringList rowdata;

    for(int i = 0; i < cols; i++){
        rowdata.append(this->model()->index(row, i).data().toString());
    }

    builder->createVarElement(newlist, &rowdata);
}
