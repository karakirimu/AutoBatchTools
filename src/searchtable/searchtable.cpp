#include "searchtable.h"

SearchTable::SearchTable(QWidget *)
{
    //disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault(QIcon(":/default_icons/copy.png"), \
                          QIcon(":/default_icons/arrow_up.png"), \
                          QIcon(":/default_icons/arrow_down.png"));
    setPopupActionBottom();

    //init table size
    setColumnCount(4);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name")
                               << tr("Return Variant") << tr("Search Word") << tr("Directory")));

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
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();

    m_edit = contextMenu->addAction(QIcon(":/default_icons/edit.png"),tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();
    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));
    connect(m_edit, SIGNAL(triggered()), this, SLOT(editAction()));
}

void SearchTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, SIGNAL(triggered()), this, SLOT(reloadAction()));
}

bool SearchTable::eventFilter(QObject *obj, QEvent *event)
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

void SearchTable::addAction()
{
    FileSearchDialog *fs = new FileSearchDialog();
    fs->setStyleSheet(this->styleSheet());
    fs->setWindowTitle(tr("Editing - untitled*"));
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
        this->setItem(row,0,new QTableWidgetItem(list->at(0).at(1)));
        this->setItem(row,1,new QTableWidgetItem(list->at(1).at(1)));
        this->setItem(row,2,new QTableWidgetItem(list->at(2).at(1)));
        this->setItem(row,3,new QTableWidgetItem(list->at(3).at(1)));
    }
    delete list;
}
