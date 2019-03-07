#include "extrafunctionstable.h"

ExtraFunctionsTable::ExtraFunctionsTable(QWidget *parent)
    : BasicTable(parent)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault(QIcon(":/default_icons/copy.png"), QIcon(":/default_icons/arrow_up.png"), QIcon(":/default_icons/arrow_down.png"));
    setPopupActionBottom();

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    //set header label
    setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Filepath")));
//    setHorizontalHeaderLabels((QStringList() << ));

    //set new xml builder
    builder = new ExtrafunctionsXmlBuilder();

    //init table (reload read file.)
    reloadAction();

    //set edit saving
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(saveAction(int)));
}

ExtraFunctionsTable::~ExtraFunctionsTable()
{
    delete builder;
}

void ExtraFunctionsTable::setPopupActionTop()
{
    //copy
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/default_icons/edit.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();
    m_file = contextMenu->addAction(QIcon(":/default_icons/file.png"), tr("Select file..."));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &ExtraFunctionsTable::addAction);
    connect(m_delete, &QAction::triggered, this, &ExtraFunctionsTable::deleteAction);
    connect(m_edit, &QAction::triggered, this, &ExtraFunctionsTable::editAction);

    connect(m_file, &QAction::triggered, this, &ExtraFunctionsTable::openFileAction);
}

void ExtraFunctionsTable::setPopupActionBottom()
{
    //copy
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, &QAction::triggered, this, &ExtraFunctionsTable::reloadAction);
}

void ExtraFunctionsTable::addAction()
{
    int row = this->rowCount();
    setRowCount(row + 1);

    //add empty item
    QList<QStringList> list;
    createList(row, &list);
    builder->addItem(&list);
}

void ExtraFunctionsTable::editAction()
{
    this->edit(currentIndex());
    return;
}

void ExtraFunctionsTable::deleteAction()
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

void ExtraFunctionsTable::reloadAction()
{
    int count = builder->count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i);
    }
}

void ExtraFunctionsTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    builder->copyItem(this->currentRow());

    reloadAction();
    selectRow(this-> rowCount() - 1);
}

void ExtraFunctionsTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    builder->swapItem(current, current-1);

    reloadAction();
    selectRow(current - 1);
}

void ExtraFunctionsTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    builder->swapItem(current, current+1);

    reloadAction();
    selectRow(current + 1);
}

void ExtraFunctionsTable::openFileAction()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString dir = dialog.getOpenFileName(this, tr("Select Extras"), "./plugins/", tr("dll (*.dll)"));

    QFileInfo info(dir);
    if(info.exists()){
        //check plugin can use
        QPluginLoader loader(dir);
        if(loader.load()){
            QObject *plugin = loader.instance();
            if(qobject_cast<ExtraPluginInterface *>(plugin)){
                //    this->setItem(this->currentRow(),1,new QTableWidgetItem(dir));
                    this->setItem(this->currentRow(), 0, new QTableWidgetItem(dir));
            }
            loader.unload();

        }else{
            QMessageBox::warning(this, tr("ProfileEditor"),
                                       tr("This dll file can not be applied."),
                                       QMessageBox::Ok);
        }

    }

}

void ExtraFunctionsTable::saveAction(int row)
{
    QList<QStringList> list;
    createList(row, &list);
    builder->editItem(row, &list);
}

void ExtraFunctionsTable::setTableItem(int row)
{
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        //set tableitem
        this->setItem(row,0,new QTableWidgetItem(list->at(0).at(1)));
//        this->setItem(row,1,new QTableWidgetItem(list->at(1).at(1)));
    }
    delete list;
}

void ExtraFunctionsTable::createList(int row, QList<QStringList> *newlist)
{
    QStringList tmp;

    //add filepath
    tmp << "extras" << this->model()->index(row, 0).data().toString();
    newlist->append(tmp);
    tmp.clear();

    //add value
//    tmp << "jsfile" << this->model()->index(row, 1).data().toString();
//    newlist->append(tmp);
//    tmp.clear();
}

bool ExtraFunctionsTable::eventFilter(QObject *obj, QEvent *event)
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
