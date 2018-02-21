#include "stringtable.h"

StringTable::StringTable(QWidget *parent) : BasicTable(parent)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault(QIcon(":/default_icons/copy.png"), QIcon(":/default_icons/arrow_up.png"), QIcon(":/default_icons/arrow_down.png"));
    setPopupActionBottom();

    //init table size
    setColumnCount(2);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Variant") << tr("Value")));

    //set new xml builder
    builder = new StringXmlBuilder();

    //init table (reload read file.)
    reloadAction();

    //set edit saving
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(saveAction(int)));
}

StringTable::~StringTable()
{
    delete builder;
}

void StringTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/default_icons/edit.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();
    m_file = contextMenu->addAction(QIcon(":/default_icons/file.png"), tr("Select File..."));
    m_dir = contextMenu->addAction(QIcon(":/default_icons/folder.png"), tr("Select Dir..."));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));
    connect(m_edit, SIGNAL(triggered()), this, SLOT(editAction()));

    connect(m_file, SIGNAL(triggered()), this, SLOT(openFileAction()));
    connect(m_dir, SIGNAL(triggered()), this, SLOT(openDirectoryAction()));
}

void StringTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, SIGNAL(triggered()), this, SLOT(reloadAction()));
}

bool StringTable::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << event->type();
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:
             if (keyEvent->modifiers() & Qt::ControlModifier)
               addAction();
             break;

           case Qt::Key_Delete:
             deleteAction();
             break;

           case Qt::Key_Up:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             }
             break;

           case Qt::Key_Down:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 downAction();
             }else{
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
             }
            break;

           case Qt::Key_C:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 copyAction();
             break;

           case Qt::Key_E:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 editAction();
             break;

           case Qt::Key_R:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                reloadAction();
             break;

           default:
             //qDebug("Ate key press %d", keyEvent->key());
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void StringTable::createList(int row, QList<QStringList> *newlist)
{
    QStringList tmp;

    //add variant
    tmp << "variant" << this->model()->index(row, 0).data().toString();
    newlist->append(tmp);
    tmp.clear();

    //add value
    tmp << "value" << this->model()->index(row, 1).data().toString();
    newlist->append(tmp);
    tmp.clear();
}

void StringTable::addAction()
{
    int row = this->rowCount();
    setRowCount(row + 1);

    //add empty item
    QList<QStringList> list;
    createList(row, &list);
    builder->addItem(&list);
}

void StringTable::editAction()
{
    this->edit(currentIndex());
    return;
}

void StringTable::deleteAction()
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

void StringTable::reloadAction()
{
    int count = builder->count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i);
    }
}

void StringTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    builder->copyItem(this->currentRow());

    reloadAction();
    selectRow(this-> rowCount() - 1);
}

void StringTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    builder->swapItem(current, current-1);

    reloadAction();
    selectRow(current - 1);
}

void StringTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    builder->swapItem(current, current+1);

    reloadAction();
    selectRow(current + 1);
}

void StringTable::setTableItem(int row)
{
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        //set tableitem
        this->setItem(row,0,new QTableWidgetItem(list->at(0).at(1)));
        this->setItem(row,1,new QTableWidgetItem(list->at(1).at(1)));
    }
    delete list;
}

void StringTable::openFileAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFile("./");
    this->setItem(current, 1, new QTableWidgetItem(str));
}

void StringTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder("./");
    this->setItem(current, 1, new QTableWidgetItem(str));
}

void StringTable::saveAction(int row)
{
    //set item
    QList<QStringList> list;
    createList(row, &list);
    builder->editItem(row, &list);
}
