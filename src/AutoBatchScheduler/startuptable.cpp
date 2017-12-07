#include "startuptable.h"

StartupTable::StartupTable(QWidget *parent)
    : BasicTable(parent)
{
    //disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault(QIcon(":/icons/Files_Copy.png"), QIcon(":/icons/Button_Up.png"), QIcon(":/icons/Button_Down.png"));
    setPopupActionBottom();

    //init table size
    setColumnCount(3);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name") << tr("Profile") << tr("Run Progress")));

    builder = new StartupXmlBuilder();

    //set doubleclick action
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editAction()));

    //init table (reload read file.)
    reloadAction();
}

StartupTable::~StartupTable()
{
    delete builder;
}

void StartupTable::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
//    connect(taskc, &TaskSchedulerConnector::xmlStateChanged, this, &StartupTable::updateItemEnabled);
    connect(taskc, &TaskSchedulerConnector::taskEnabled, this, &StartupTable::updateItemEnabled);
    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &StartupTable::updateItemEnabled);
//    connect(this, &StartupTable::actionDeleted, task, &TaskSchedulerConnector::actionDeleted);
//    connect(this, &StartupTable::actionAdded, task, &TaskSchedulerConnector::actionAdded);
}

void StartupTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/icons/Add.png"), tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/icons/Denided.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
    m_edit = contextMenu->addAction(QIcon(":/icons/Pen.png"), tr("Edit"));
    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    contextMenu->addSeparator();
    m_enable = contextMenu->addAction(QIcon(":/icons/Valid.png"), tr("Enable"));
    m_disable = contextMenu->addAction(QIcon(":/icons/Error.png"), tr("Disable"));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_edit, SIGNAL(triggered()), this, SLOT(editAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));

    connect(m_enable, SIGNAL(triggered()), this, SLOT(enableAction()));
    connect(m_disable, SIGNAL(triggered()), this, SLOT(disableAction()));
}

void StartupTable::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/icons/arrow_refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, SIGNAL(triggered()), this, SLOT(reloadAction()));
}

bool StartupTable::eventFilter(QObject *obj, QEvent *event)
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

int StartupTable::getStartupXmlIndex(QString objectname)
{
    //copy of systemtray
    QList<QStringList> *list = new QList<QStringList>();

    //search valid data
    int itemid = -1;
    int count = builder->count();

    for(int i = 0; i < count; i++){
        list->clear();
        if(builder->readItem(i, list)
                && objectname == list->at(StartupXmlBuilder::UNIQUE).at(1)){
            itemid = i;
            break;
        }
    }

    delete list;
    return itemid;
}

void StartupTable::setTableItem(int row)
{
    //qDebug () << "setTableItem";
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        //set tableitem
        this->setItem(row,0,new QTableWidgetItem(list->at(0).at(1)));
        this->setItem(row,1,new QTableWidgetItem(list->at(1).at(1)));

        //set icon
        QString icon;
        if(list->at(2).at(1) == "yes"){
            icon = ":/icons/Valid.png";
        }else{
            icon = ":/icons/Denided.png";
        }

        this->setItem(row,2,new QTableWidgetItem(QIcon(icon),list->at(2).at(1)));
    }
    delete list;
}

void StartupTable::addAction()
{
    StartupDialog *sd = new StartupDialog();
    sd->setWindowTitle(tr("Editing-New_file*"));
    if(sd->exec() == QDialog::Accepted){
        int index = this->rowCount();
        setRowCount(index + 1);
        setTableItem(index);

        QList<QStringList> list;
        if(builder->readItem(currentRow(), &list)
                && list.at(StartupXmlBuilder::VALID).at(1) == "yes"){
            QFileInfo info(list.at(StartupXmlBuilder::PROF).at(1));
            if(info.exists()) taskc->enableTask(list.at(StartupXmlBuilder::UNIQUE).at(1), info.canonicalFilePath());
        }
    }
}

void StartupTable::editAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QString yesno = "";
    StartupDialog *sd = new StartupDialog();
    QList<QStringList> list;
    int row = currentRow();
    if(builder->readItem(row, &list)){
        //set title
        sd->loadSettingList(row, &list);
        yesno = list.at(StartupXmlBuilder::VALID).at(1);
    }
    if(sd->exec() == QDialog::Accepted){
        setTableItem(row);

        if(yesno == "yes"){
            //disable
            if(builder->readItem(row, &list)){
                if(list.at(StartupXmlBuilder::VALID).at(1) == "no"){
                    taskc->disableTask(list.at(StartupXmlBuilder::UNIQUE).at(1));
                }
            }
        }else{
            //enable
            if(builder->readItem(row, &list)){
                if(list.at(StartupXmlBuilder::VALID).at(1) == "yes"){
                    QFileInfo info(list.at(StartupXmlBuilder::PROF).at(1));
                    if(info.exists()) taskc->enableTask(list.at(StartupXmlBuilder::UNIQUE).at(1), info.canonicalFilePath());

                }
            }
        }
    }
}

void StartupTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
    {
        QList<QStringList> list;
        if(builder->readItem(currentRow(), &list)
                && list.at(StartupXmlBuilder::VALID).at(1) == "yes"){
            taskc->disableTask(list.at(StartupXmlBuilder::UNIQUE).at(1));
        }

        //delete file item
        builder->deleteItem(currentRow());

        //reload
        reloadAction();
    }
}

void StartupTable::reloadAction()
{
    int count = builder->count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i);
    }
}

void StartupTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    builder->copyItem(this->currentRow());

    reloadAction();

    selectRow(this-> rowCount() - 1);
}

void StartupTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    builder->swapItem(current, current-1);

    reloadAction();

    selectRow(current - 1);
}

void StartupTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    builder->swapItem(current, current+1);

    reloadAction();

    selectRow(current + 1);
}

void StartupTable::enableAction(){

    QList<QStringList> *list = new QList<QStringList>();
    int row = this->currentRow();

    if(builder->readItem(row, list)){
        if(list->at(StartupXmlBuilder::VALID).at(1) == "no"){

            //change validation
            QStringList tmp;
            tmp << "valid" << "yes";
            list->removeAt(2);
            list->insert(2, tmp);

            builder->editItem(row, list);

//            emit taskc->xmlStateChanged(list->at(StartupXmlBuilder::UNIQUE).at(1));
            reloadAction();

            QFileInfo info(list->at(StartupXmlBuilder::PROF).at(1));
            if(info.exists()){
                taskc->enableTask(list->at(StartupXmlBuilder::UNIQUE).at(1), info.canonicalFilePath());
            }
        }
    }
}

void StartupTable::disableAction(){

    QList<QStringList> *list = new QList<QStringList>();
    int row = this->currentRow();

    if(builder->readItem(row, list)){
        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"){

            //change validation
            QStringList tmp;
            tmp << "valid" << "no";
            list->removeAt(2);
            list->insert(2, tmp);

            builder->editItem(row, list);

//            emit taskc->xmlStateChanged(list->at(StartupXmlBuilder::UNIQUE).at(1));
            reloadAction();
            taskc->disableTask(list->at(StartupXmlBuilder::UNIQUE).at(1));
        }
    }
}

void StartupTable::updateItemEnabled(QString objectname)
{
    int itemid = getStartupXmlIndex(objectname);
    if(itemid > -1) setTableItem(itemid);
}
