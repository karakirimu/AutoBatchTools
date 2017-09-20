#include "varianttable.h"

VariantTable::VariantTable(QWidget *)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault(QIcon(":/icons/Files_Copy.png"), QIcon(":/icons/Button_Up.png"), QIcon(":/icons/Button_Down.png"));
    setPopupActionBottom();

    //init table size
    setColumnCount(2);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //hide row numbers
    verticalHeader()->setVisible(false);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Variant") << tr("Value")));

    //set edit saving
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(editAction()));
}

VariantTable::~VariantTable()
{

}

void VariantTable::setSharedFunction(PESharedFunction *func)
{
    sfunction = func;
}

void VariantTable::addAction()
{
    QList<QStringList> newlist;
    int row = this->rowCount();
    setRowCount(row + 1);
    getLocalList(&newlist);
    sfunction->editItem(MAGIC, &newlist);
}

void VariantTable::editAction()
{
    QList<QStringList> newlist;
    getLocalList(&newlist);
    sfunction->editItem(MAGIC, &newlist);
}

void VariantTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
    {
        //delete file item
        this->deleteTableRecursive();
    }
}

void VariantTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //item copy
    int current = this->currentRow();
    this->insertRow(current + 1);
    //set tableitem
    this->setItem(current + 1, 0, new QTableWidgetItem(this->model()->index(current, 0).data().toString()));
    this->setItem(current + 1, 1, new QTableWidgetItem(this->model()->index(current, 1).data().toString()));

    selectRow(current + 1);

    //save to backup file
    editAction();
}

void VariantTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    tableItemSwap(current, current-1);

    reloadAction();
    selectRow(current - 1);
}

void VariantTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    tableItemSwap(current, current+1);

    reloadAction();
    selectRow(current + 1);
}

void VariantTable::reloadAction()
{
    qDebug() << "reloadaction";
    setRowCount(0);
    setLocalListItem(MAGIC);
}

void VariantTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/icons/Add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/icons/Denided.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/icons/Pen.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));
    connect(m_edit, SIGNAL(triggered()), this, SLOT(editAction()));
}

void VariantTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/icons/arrow_refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, SIGNAL(triggered()), this, SLOT(reloadAction()));
}

bool VariantTable::eventFilter(QObject *obj, QEvent *event)
{
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
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

bool VariantTable::setLocalListItem(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    if(!sfunction->readItem(itemid, list)){
        delete list;
        return false;
    }

    //get type
    QString type = list->at(0).at(1);

    //set root
    if(type == "local"){
        int counter = list->count();
        setRowCount(counter - 2);
        for(int i = 2; i < counter; i++){
            //set tableitem
            this->setItem(i - 2, 0, new QTableWidgetItem(list->at(i).at(1)));
            this->setItem(i - 2, 1, new QTableWidgetItem(list->at(i).at(3)));
        }

        qDebug()<< "setTreeItem";
        delete list;
        return true;
    }

    return false;
}

void VariantTable::tableItemSwap(int from, int to)
{
    QString bku0 = this->model()->index(from, 0).data().toString();
    QString bku1 = this->model()->index(from, 1).data().toString();

    //set tableitem
    this->setItem(from, 0, new QTableWidgetItem(this->model()->index(to, 0).data().toString()));
    this->setItem(from, 1, new QTableWidgetItem(this->model()->index(to, 1).data().toString()));
    //set tableitem2
    this->setItem(to, 0, new QTableWidgetItem(bku0));
    this->setItem(to, 1, new QTableWidgetItem(bku1));
}

void VariantTable::getLocalList(QList<QStringList> *newlist)
{
    QStringList list;

    int rcount = this->rowCount();
    list.append(QString::number(rcount));
    for(int i = 0; i < rcount; i++){
        list.append(this->model()->index(i, 0).data().toString());
        list.append(this->model()->index(i, 1).data().toString());
    }

    sfunction->createLocalList(newlist, &list);
}
