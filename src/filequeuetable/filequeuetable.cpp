#include "filequeuetable.h"


FileQueueTable::FileQueueTable(QWidget *parent)
    : BasicTable(parent)
{
    // disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    setHorizontalHeaderLabels((QStringList() << tr("FilePath")));

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set header sort settings
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(horizontalHeaderClicked(int)));

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

void FileQueueTable::deleteAciton()
{
    deleteTableRecursive();
}

void FileQueueTable::clearAction()
{
    this->selectAll();
    deleteTableRecursive();
//    this->setRowCount(0);
}

void FileQueueTable::propertyAction()
{
    if(this->rowCount() > 0){
        FileInfoDialog *dialog = new FileInfoDialog();
        dialog->setFileInfo(this->selectedItems().at(0)->text());
        dialog->show();
    }
}

void FileQueueTable::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}

void FileQueueTable::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
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

            //テーブルに読み込んだ情報を追加
            QFileInfo file(filenames.at(i));
           this->setItem(rcount,0,new QTableWidgetItem(file.canonicalFilePath()));
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
    m_add = contextMenu->addAction(QIcon(":/default_icons/file.png"), tr("Add Files"));
    m_deleteitems = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Clear this Item"));
    contextMenu->addSeparator();
    m_deleteAll = contextMenu->addAction(tr("Clear All Items"));

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_deleteitems, SIGNAL(triggered()), this, SLOT(deleteAciton()));
    connect(m_deleteAll, SIGNAL(triggered()), this, SLOT(clearAction()));
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
    m_property = contextMenu->addAction(tr("File Info"));
    connect(m_property, SIGNAL(triggered()), this, SLOT(propertyAction()));
}

void FileQueueTable::addAction()
{
    QStringList selected = selectFiles("./");
    addFiles(selected);
}
