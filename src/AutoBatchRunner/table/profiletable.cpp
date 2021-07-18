/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "profiletable.h"

ProfileTable::ProfileTable(QWidget *parent)
    : BasicTable(parent)
{
    //disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //popupAction
    setPopupActionTop();

    //init table size
    setColumnCount(3);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //set vertical header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                    , qpc.VERTICAL_HEADER_ENABLE);

    //set header label
    setHorizontalHeaderLabels((QStringList()
                               << tr("Profile")
                               << tr("Description")
                               << tr("File")));

    //set new xml builder
    builder = new ProfileXmlBuilder();

    //init table (reload read file.)
    reloadAction();

    //set doubleclick action
    connect(this, &ProfileTable::cellDoubleClicked
            , this, &ProfileTable::launchEditor);
}

ProfileTable::~ProfileTable()
{
    delete builder;
}

void ProfileTable::setPopupActionTop()
{
    //set basic items
    m_new = addTableAction(ACTION::NEWFILE);
    m_add = addTableAction(ACTION::ADD, Qt::CTRL, Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL, Qt::Key_E);
    contextMenu->addSeparator();

    m_up = addTableAction(ACTION::UP, Qt::CTRL, Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL, Qt::Key_Down);
    contextMenu->addSeparator();

    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL, Qt::Key_R);

    //connect signals
    connect(m_new, &QAction::triggered, this, &ProfileTable::newAction);
    connect(m_add, &QAction::triggered, this, &ProfileTable::addAction);
    connect(m_edit, &QAction::triggered, this, &ProfileTable::editAction);
    connect(m_delete, &QAction::triggered, this, &ProfileTable::deleteAction);

    connect(m_up, &QAction::triggered, this, &ProfileTable::upAction);
    connect(m_down, &QAction::triggered, this, &ProfileTable::downAction);

    connect(m_ref, &QAction::triggered, this, &ProfileTable::reloadAction);
}

bool ProfileTable::eventFilter(QObject *obj, QEvent *event)
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

          case Qt::Key_E:      if (mdCheck())  editAction();    break;
          case Qt::Key_R:      if (mdCheck())  reloadAction();  break;
          default:                                              break;
        }
       return true;
   }
   // standard event processing
   return QObject::eventFilter(obj, event);
}

void ProfileTable::createListFromXml(QString filename
                                     , QList<QStringList> *newlist)
{
    QList<QStringList> item;

    //open processxmlbuilder
    ProcessXmlBuilder *pbuilder = new ProcessXmlBuilder();

    //set file to read
    pbuilder->setLoadPath(filename);

    //read item 0 (info)
    if(pbuilder->readItem(0, &item)){
        QStringList vl = (QStringList()
                          << xf.fetch(&item, pxc.TAG_I_NAME)
                          << xf.fetch(&item, pxc.TAG_I_DESCRIPTION)
                          << filename);

        builder->createVarElement(newlist, &vl);
    }
    delete pbuilder;
}

/**
 * @fn ProfileTable::createList
 * @brief Create a list from table data to be written to XML.
 * @param row Selected line.
 * @param newlist Conversion result.
 */
void ProfileTable::createList(int row
                              , QList<QStringList> *newlist)
{
    int cols = this->columnCount();

    QStringList rowdata;
    for(int i = 0; i < cols; i++){
        if(i == 2){
            // need fullpath information
            rowdata.append(this->model()->index(row, i)
                               .data(Qt::UserRole).toString());
        }else{
            rowdata.append(this->model()->index(row, i)
                               .data().toString());
        }
    }

    builder->createVarElement(newlist, &rowdata);
}

/**
 * @fn ProfileTable::newAction
 * @brief Start AutoBatchEditor
 */
void ProfileTable::newAction()
{
    bool result = QProcess().startDetached("./AutoBatchEditor");

    qDebug() << "[ProfileTable::newAction] AutoBatchEditor launched:"
             << result;
}

/**
 * @fn ProfileTable::addAction
 * @brief Add a table and xml elements containing an empty string.
 */
void ProfileTable::addAction()
{
    QString fileName =
            QFileDialog::getOpenFileName(this
                                     , tr("Open Profile")
                                     , QDir::currentPath()
                                     , tr("Profile ") + "(*.xml *.apro)");
    if (fileName.isEmpty()) return;

    // add new item
    QList<QStringList> list;
    createListFromXml(fileName, &list);
    builder->addItem(&list);

    reloadAction();
}

/**
 * @fn ProfileTable::editAction
 * @brief Launch AutoBatchEditor for the selected location.
 */
void ProfileTable::editAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;
    launchEditor(this->currentRow());
}

/**
 * @fn ProfileTable::deleteAction
 * @brief Deletes the selected tableItem and xml elements synchronously.
 */
void ProfileTable::deleteAction()
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
 * @fn ProfileTable::upAction
 * @brief Move up one selection line.
 */
void ProfileTable::upAction()
{
    upSelected([&](int current) {
        Q_UNUSED(current)
        updateXml();
    });
}

/**
 * @fn ProfileTable::downAction
 * @brief Move down one selection line.
 */
void ProfileTable::downAction()
{
    downSelected([&](int current) {
        Q_UNUSED(current)
        updateXml();
    });
}

/**
 * @fn ProfileTable::reloadAction
 * @brief Update all table items.
 */
void ProfileTable::reloadAction()
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
 * @fn ProfileTable::launchEditor
 * @brief Load the selected file and start AutoBatchEditor.
 * @param row selected row
 * @param col selected column
 */
void ProfileTable::launchEditor(int row, int col)
{
    Q_UNUSED(col)

    QString path = this->model()
                       ->index(row, 2)
                       .data(Qt::UserRole).toString();

    bool result = QProcess().startDetached("./AutoBatchEditor"
                                           , QStringList() << path);

    qDebug() << "[ProfileTable::launchEditor] AutoBatchEditor launched:"
             << result;
}

/**
 * @fn ProfileTable::updateXml
 * @brief It takes the data in the table and overwrites the XML file.
 */
void ProfileTable::updateXml()
{
    int datacount = this->rowCount();
    QList<QList<QStringList> *> itemlist;

    for(int i = 0; i < datacount; i++){
        //add empty item
        QList<QStringList> *list = new QList<QStringList>();
        createList(i, list);
        itemlist.append(list);
    }

    builder->writeAll(&itemlist);
}

/**
 * @fn ProfileTable::setTableItem
 * @brief
 * Set data for a single row.
 * The second column contains the file name
 * in Qt::DisplayRole and the file path in Qt::UserRole.
 *
 * @param row Row to set the table.
 * @param item One line of data to retrieve from XML.
 */
void ProfileTable::setTableItem(int row, const QList<QStringList> *item)
{
    // set item
    this->setItem(row, 0, new QTableWidgetItem(item->at(0).at(1)));
    this->setItem(row, 1, new QTableWidgetItem(item->at(1).at(1)));

    // set display name and filepath for reload
    QFileInfo info(item->at(2).at(1));
    QTableWidgetItem *file = new QTableWidgetItem(info.fileName());
    file->setData(Qt::UserRole, QVariant(item->at(2).at(1)));
    this->setItem(row, 2, file);
}
