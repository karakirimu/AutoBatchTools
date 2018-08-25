#include "varianttable.h"

VariantTable::VariantTable(QWidget *)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
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
    setHorizontalHeaderLabels((QStringList() << tr("File Variant") << tr("Value")));

    //set edit saving
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(editAction()));
}

VariantTable::~VariantTable()
{

}

//void VariantTable::setSharedFunction(PESharedFunction *func)
//{
//    sfunction = func;
//}

void VariantTable::setEditOperator(EditOperator *op)
{
    editop = op;
}

//ALLOC MEMORY
void VariantTable::addAction()
{
    QList<QStringList> *newlist = new QList<QStringList>;
    int row = this->rowCount();
    setRowCount(row + 1);
    getLocalList(newlist);
    editop->tableEditVariantAction(MAGIC, newlist);
}

//ALLOC MEMORY
void VariantTable::editAction()
{
    QList<QStringList> *newlist = new QList<QStringList>;
    getLocalList(newlist);
    editop->tableEditVariantAction(MAGIC, newlist);
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

void VariantTable::cutAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    copyAction();

    //force delete
    BaseTable::deleteTableRecursive();

    //save to backup file
    editAction();
}

void VariantTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //item copy
//    int current = this->currentRow();
//    this->insertRow(current + 1);
//    //set tableitem
//    this->setItem(current + 1, 0, new QTableWidgetItem(this->model()->index(current, 0).data().toString()));
//    this->setItem(current + 1, 1, new QTableWidgetItem(this->model()->index(current, 1).data().toString()));

//    selectRow(current + 1);

    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();

    // 2 column
    int counter = mlist.count();
    for(int i = 0; i < counter; i++){
        int crow = mlist.at(i).row();

        tmp.append(mlist.at(i).model()->index(crow, i%2).data().toString());

        if(i%2 == 0){
            tmp.append("\t");
        }else{
            tmp.append("\n");
        }
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);

//    //save to backup file
//    editAction();
}

void VariantTable::pasteAction()
{
    //modified CommandTable::pasteEnterAction()
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(QRegularExpression("\\n|\\r\\n"));

    //last lests unknown ""
    if(text.count() > 1) text.removeLast();

    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);
       QStringList intext = ((QString)text.at(i)).split(QRegularExpression("\\t|,"));

       int intxt = intext.count();
       if(intxt > 0){
           this->setItem(row, 0, new QTableWidgetItem(intext.at(0)));
           if(intxt > 1){
               this->setItem(row, 1, new QTableWidgetItem(intext.at(1)));
           }
       }
//       emit updateTable(row, text.at(i), ProcessXmlListGenerator::TABLE_INSERT);
    }

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
    qDebug() << "VariantTable::reloadaction";
    disconnect(this, SIGNAL(cellChanged(int,int)), this, SLOT(editAction()));
    setRowCount(0);
    setLocalListItem(MAGIC);
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(editAction()));
}

void VariantTable::setPopupActionTop()
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

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));
    connect(m_edit, SIGNAL(triggered()), this, SLOT(editAction()));
}

void VariantTable::setPopupActionDefault()
{
    //set basic items
    m_cut = contextMenu->addAction(QIcon(":/default_icons/cut.png"), tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/default_icons/copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/default_icons/paste.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    contextMenu->addSeparator();

    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    //connect signals
    connect(m_cut, SIGNAL(triggered()), this, SLOT(cutAction()));
    connect(m_copy, SIGNAL(triggered()), this, SLOT(copyAction()));
    connect(m_paste, SIGNAL(triggered()), this, SLOT(pasteAction()));
    connect(m_up, SIGNAL(triggered()), this, SLOT(upAction()));
    connect(m_down, SIGNAL(triggered()), this, SLOT(downAction()));
}

void VariantTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
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

           case Qt::Key_X:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 cutAction();
             break;

           case Qt::Key_C:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 copyAction();
             break;

           case Qt::Key_V:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 pasteAction();
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

//DEPENDS_XML OUTPUT PROCESS
bool VariantTable::setLocalListItem(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    if(!editop->read(itemid, list)){
        delete list;
        return false;
    }

    //get type
    QString type = list->at(0).at(1);

    //set root
    if(type == "local"){
        int counter = QString(list->at(1).at(1)).toInt();
        setRowCount(counter);
        for(int i = 0; i < counter; i++){
            //set tableitem
            this->setItem(i, 0, new QTableWidgetItem(list->at(i+2).at(1)));
            this->setItem(i, 1, new QTableWidgetItem(list->at(i+2).at(3)));
        }

        qDebug()<< "VariantTable::setTreeItem";
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

//DEPENDS_XML INPUT PROCESS
void VariantTable::getLocalList(QList<QStringList> *newlist)
{
    QStringList list;

    int rcount = this->rowCount();
    list.append(QString::number(rcount));
    for(int i = 0; i < rcount; i++){
        list.append(this->model()->index(i, 0).data().toString());
        list.append(this->model()->index(i, 1).data().toString());
    }

    xgen.createLocalList(newlist, &list);
}
