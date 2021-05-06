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

#include "filequeuetable.h"


FileQueueTable::FileQueueTable(QWidget *parent)
    : BasicTable(parent)
{
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &FileQueueTable::horizontalHeaderClicked);

}

FileQueueTable::~FileQueueTable()
{

}

void FileQueueTable::horizontalHeaderClicked(int column){
    Qt::SortOrder order = horizontalHeader()->sortIndicatorOrder() ? Qt::DescendingOrder : Qt::AscendingOrder;
    sortItems(column, order);
}

void FileQueueTable::dropEvent(QDropEvent *event)
{
    const QMimeData* mime = event->mimeData();
    qDebug() << event->mimeData()->hasUrls();
    if(mime->hasUrls()){
        addFiles(droppedFromOutside(event));
    }else {
        insideDropRowsMove(event);
    }
}

void FileQueueTable::addFilesAction()
{
    QStringList selected;

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    selected = dialog.getOpenFileNames(this, tr("Open Files"),\
                                       QDir::currentPath(), tr("Files (*.*)"));
    addFiles(selected);
}

void FileQueueTable::addFolderAction()
{
    QString selected = selectFolder(QDir::currentPath());

    int rcount = this->rowCount();
    this->setRowCount(rcount+1);
    this->setItem(rcount,0,new QTableWidgetItem(selected));
}

void FileQueueTable::deleteAciton()
{
    deleteTableRecursive();
}

void FileQueueTable::clearAction()
{
    this->selectAll();
    deleteTableRecursive();
}

void FileQueueTable::propertyAction()
{
    if(this->rowCount() > 0){
        FileInfoDialog *dialog = new FileInfoDialog();
        dialog->setStyleSheet(this->styleSheet());
        dialog->setFileInfo(this->selectedItems().at(0)->text());
        dialog->show();
    }
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
    // check files
    for (int i = 0; i < filenames.size(); ++i){
        // get current row count
        int rcount = this->rowCount();
        //open file
        QFile cfile(filenames.at(i));
        if (cfile.open(QIODevice::ReadOnly)){
            // append row
            this->setRowCount(rcount+1);

            //Add imported information to table
            QFileInfo file(filenames.at(i));
            QTableWidgetItem *newitem = new QTableWidgetItem();
            newitem->setData(Qt::DisplayRole, file.canonicalFilePath());
            newitem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled \
                              | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

           this->setItem(rcount,0, newitem);
        }
        cfile.close();
    }
}

void FileQueueTable::setPopupActionTop()
{
    //init menu context
//    contextMenu->setStyleSheet("border:1px;");
    m_addfile = addTableAction(ACTION::FILES);
    m_adddir = addTableAction(ACTION::FOLDER);
    m_deleteitems = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();
    m_deleteAll = addTableAction(ACTION::CLEAR);

    //connect signals
    connect(m_addfile, &QAction::triggered, this, &FileQueueTable::addFilesAction);
    connect(m_adddir, &QAction::triggered, this, &FileQueueTable::addFolderAction);
    connect(m_deleteitems, &QAction::triggered, this, &FileQueueTable::deleteAciton);
    connect(m_deleteAll, &QAction::triggered, this, &FileQueueTable::clearAction);
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
    connect(m_property, &QAction::triggered, this, &FileQueueTable::propertyAction);
}
