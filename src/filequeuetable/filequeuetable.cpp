/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "filequeuetable.h"


FileQueueTable::FileQueueTable(QWidget *parent)
    : BasicTable(parent)
{
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // set drop from outside
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::CopyAction);
    setDragDropMode(QAbstractItemView::DragDrop);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("File / Folder Path")));

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                  , qpc.VERTICAL_HEADER_ENABLE);

    //set header sort settings
    connect(horizontalHeader()
            , &QHeaderView::sectionClicked
            , this
            , &FileQueueTable::horizontalHeaderClicked);

}

FileQueueTable::~FileQueueTable()
{

}

void FileQueueTable::horizontalHeaderClicked(int column){
    Qt::SortOrder order
        = horizontalHeader()->sortIndicatorOrder()
              ? Qt::DescendingOrder : Qt::AscendingOrder;
    sortItems(column, order);
}

void FileQueueTable::dropEvent(QDropEvent *event)
{
    const QMimeData* mime = event->mimeData();

    qDebug() << "[FileQueueTable::dropEvent] hasUrls :"
             << mime->hasUrls();

    if(mime->hasUrls()){
        addFiles(droppedFromOutside(event));
    }else {
        int droppedrow
            = this->indexAt(event->position().toPoint()).row();
        droppedFromInside(droppedrow);
    }
}

void FileQueueTable::addFilesAction()
{
    QStringList selected;

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    selected
        = dialog.getOpenFileNames(this
                                  , tr("Open Files")
                                  , QDir::currentPath()
                                  , tr("Files (*.*)"));
    addFiles(selected);
}

void FileQueueTable::addFolderAction()
{
    QString selected = selectFolder(QDir::currentPath());

    int rcount = this->rowCount();
    this->setRowCount(rcount + 1);
    QIcon folder = QFileIconProvider().icon(QAbstractFileIconProvider::Folder);
    this->setItem(rcount, 0, new QTableWidgetItem(folder, selected));
}

void FileQueueTable::deleteAciton()
{
    deleteTableRecursive();
}

void FileQueueTable::clearAction()
{
    this->clearContents();
    this->setRowCount(0);
}

void FileQueueTable::propertyAction()
{
    if(this->selectionModel()
            ->selectedRows().count() != 1){
        return;
    }

    FileInfoDialog *dialog = new FileInfoDialog();
    dialog->setStyleSheet(this->styleSheet());
    dialog->setFileInfo(this->selectedItems().at(0)->text());
    dialog->show();
}

void FileQueueTable::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()
            || event->source() != nullptr){
        event->acceptProposedAction();
    }
}

void FileQueueTable::addFiles(const QStringList &filenames)
{
    for(const auto &filename : filenames){
        // get current row count
        int rcount = this->rowCount();
        //open file
        QFile cfile(filename);
        if (cfile.open(QIODevice::ReadOnly)){
            // append row
            this->setRowCount(rcount + 1);

            //Add imported information to table
            QFileInfo file(filename);
            QTableWidgetItem *newitem = new QTableWidgetItem();
            newitem->setData(Qt::DisplayRole, file.canonicalFilePath());


#ifdef Q_OS_WINDOWS
            QFileIconProvider provider;
            QIcon icon = provider
                           .icon(QAbstractFileIconProvider::File);
#else
            QFileIconProvider provider;
                        QIcon icon = provider.icon(file);

            if(icon.isNull()){
            QIcon icon = provider
                             .icon(QAbstractFileIconProvider::File);
            }
#endif
            newitem->setIcon(icon);

            newitem->setFlags(Qt::ItemIsSelectable
                              | Qt::ItemIsEnabled
                              | Qt::ItemIsDragEnabled
                              | Qt::ItemIsDropEnabled);
            this->setItem(rcount, 0, newitem);
        }
        cfile.close();
    }
}

void FileQueueTable::setPopupActionTop()
{
    //init menu context
    m_addfile = addTableAction(ACTION::FILES);
    m_adddir = addTableAction(ACTION::FOLDER);
    m_deleteitems = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();
    m_deleteAll = addTableAction(ACTION::CLEAR);

    //connect signals
    connect(m_addfile, &QAction::triggered
            , this, &FileQueueTable::addFilesAction);
    connect(m_adddir, &QAction::triggered
            , this, &FileQueueTable::addFolderAction);
    connect(m_deleteitems, &QAction::triggered
            , this, &FileQueueTable::deleteAciton);
    connect(m_deleteAll, &QAction::triggered
            , this, &FileQueueTable::clearAction);
}

void FileQueueTable::setPopupActionDefault()
{
    contextMenu->addSeparator();
    //set basic items
    m_up = addTableAction(ACTION::UP, Qt::CTRL, Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL, Qt::Key_Down);

    //connect signals
    connect(m_up, &QAction::triggered, this, &FileQueueTable::upAction);
    connect(m_down, &QAction::triggered, this, &FileQueueTable::downAction);
}

void FileQueueTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_property = addTableAction(ACTION::FILEINFO);

    connect(m_property, &QAction::triggered
            , this, &FileQueueTable::propertyAction);
}
