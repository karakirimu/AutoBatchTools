#include "filequeuetable.h"


FileQueueTable::FileQueueTable(QWidget *parent)
    : BasicTable(parent)
{
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);
//    setDragDropMode(QAbstractItemView::InternalMove);

    //accept drops
//    setAcceptDrops(true);
//    setDragDropMode(QAbstractItemView::DragDrop);
//    setDropIndicatorShown(true);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("File/Folder Path")));

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

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
        droppedFromInside(event);
    }
}

void FileQueueTable::addFilesAction()
{
    QStringList selected = selectFiles(QDir::currentPath());
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

//void FileQueueTable::dragMoveEvent(QDragMoveEvent *event)
//{
//    event->acceptProposedAction();
//}

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
            //test
//            this->setItem(rcount,1,new QTableWidgetItem(QString::number(i)));
        }
        cfile.close();
    }
}

void FileQueueTable::setPopupActionTop()
{
    //init menu context
    contextMenu->setStyleSheet("border:1px;");
    m_addfile = contextMenu->addAction(QIcon(":/default_icons/file.png"), tr("Add Files ..."));
    m_adddir = contextMenu->addAction(QIcon(":/default_icons/folder.png"), tr("Add Folder ..."));
    m_deleteitems = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    contextMenu->addSeparator();
    m_deleteAll = contextMenu->addAction(tr("Clear"));

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
    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));

    //connect signals
    connect(m_up, SIGNAL(triggered()), this, SLOT(upAction()));
    connect(m_down, SIGNAL(triggered()), this, SLOT(downAction()));
}

void FileQueueTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_property = contextMenu->addAction(QIcon(":/default_icons/info.png"), tr("File Info"));
    connect(m_property, &QAction::triggered, this, &FileQueueTable::propertyAction);
}
