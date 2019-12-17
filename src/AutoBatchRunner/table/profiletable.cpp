﻿#include "profiletable.h"

ProfileTable::ProfileTable(QWidget *)
{
    //disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault(getIcon(ACTION::COPY), \
                          getIcon(ACTION::UP), \
                          getIcon(ACTION::DOWN));

    //init table size
    setColumnCount(3);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name") << tr("Description") << tr("Filename")));

    //set new xml builder
    builder = new ProfileXmlBuilder();

    //disable copy context
    m_copy->setEnabled(false);

    //init table (reload read file.)
    reloadAction();

    //set doubleclick action
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editAction()));
}

ProfileTable::~ProfileTable()
{
    delete builder;
}

void ProfileTable::newAction()
{
    //run ProfileEditor.exe
    QProcess process;
#ifdef QT_DEBUG
    bool result = process.startDetached(tr("./ProfileEditor.exe"));
    if(!result) qDebug() << tr("ProfileEditor launch failed.");
#else
    process.startDetached(tr("./ProfileEditor.exe"));
#endif
}

void ProfileTable::setPopupActionTop()
{
    //set basic items
    m_new = addTableAction(ACTION::NEWFILE);
    m_add = addTableAction(ACTION::ADD, Qt::CTRL + Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL + Qt::Key_E);
    contextMenu->addSeparator();

    //connect signals
    connect(m_new, &QAction::triggered, this, &ProfileTable::newAction);
    connect(m_add, &QAction::triggered, this, &ProfileTable::addAction);
    connect(m_edit, &QAction::triggered, this, &ProfileTable::editAction);
    connect(m_delete, &QAction::triggered, this, &ProfileTable::deleteAction);
}

bool ProfileTable::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

   auto mdCheck = [&keyEvent](){
       return static_cast<bool>(keyEvent->modifiers() & Qt::ControlModifier);
   };

   //qDebug() << event->type();
   if (event->type() == QEvent::KeyPress) {
       keyEvent = static_cast<QKeyEvent *>(event);
       switch (keyEvent->key())
        {
          case Qt::Key_Return:
          case Qt::Key_Enter:  if (mdCheck())  addAction();     break;
          case Qt::Key_Delete:                 deleteAction();  break;

          case Qt::Key_Up:
            if (mdCheck()){
                upAction();
            }else{
                if(this->currentRow() != 0)
                    selectRow(this->currentRow() - 1);
            }
            break;

          case Qt::Key_Down:
            if (mdCheck()){
                downAction();
            }else{
                if(this->rowCount() - 1 != this->currentRow())
                    selectRow(this->currentRow() + 1);
            }
           break;

          case Qt::Key_C:      if (mdCheck())  copyAction();    break;
          case Qt::Key_E:      if (mdCheck())  editAction();    break;
          case Qt::Key_R:      if (mdCheck())  reloadAction();  break;

          default:
            //qDebug("Ate key press %d", keyEvent->key());
            break;
        }
       return true;
   }
   // standard event processing
   return QObject::eventFilter(obj, event);
}

void ProfileTable::createList(QString filename, QList<QStringList> *newlist)
{
    QStringList tmp;
    QList<QStringList> item;

    //open profileeditorxmlbuilder
    ProcessXmlBuilder *pbuilder = new ProcessXmlBuilder();

    //set file to read
    pbuilder->setLoadPath(filename);

    //read item 0 (info)
    if(pbuilder->readItem(0, &item)){
        //add name
        newlist->append((QStringList() << "name" << item.at(1).at(1)));

        //add desc
        newlist->append((QStringList() << "desc" << item.at(4).at(1)));

        //add filepath
        newlist->append((QStringList() << "file" << filename));
    }
    delete pbuilder;
}

void ProfileTable::setTableItem(int row)
{
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        //set tableitem
        this->setItem(row,0,new QTableWidgetItem(list->at(0).at(1)));
        this->setItem(row,1,new QTableWidgetItem(list->at(1).at(1)));
        QFileInfo info(list->at(2).at(1));
        this->setItem(row,2,new QTableWidgetItem(info.fileName()));
    }
    delete list;
}

void ProfileTable::addAction()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open Profile"),\
                                         QDir::currentPath(),\
                                         tr("Profile (*.xml *.apro)"));
    if (fileName.isEmpty())
        return;

    //add new item
    QList<QStringList> list;
    createList(fileName, &list);
    builder->addItem(&list);

    reloadAction();
}

void ProfileTable::editAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(this->currentRow(), list)){

        QProcess process;
#ifdef QT_DEBUG
        bool result = process.startDetached("./ProfileEditor.exe", \
                        (QStringList() << list->at(2).at(1)));
       if(!result) qDebug() << tr("ProfileEditor launch failed.");
#else
    #ifdef Q_OS_WIN
        process.startDetached("./ProfileEditor.exe", QStringList() << list->at(2).at(1));
    #else
        process.startDetached("./ProfileEditor", QStringList() << list->at(2).at(1));
    #endif
#endif
    }
    delete list;
}

void ProfileTable::deleteAction()
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

void ProfileTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    builder->swapItem(current, current-1);

    reloadAction();

    selectRow(current - 1);
}

void ProfileTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    builder->swapItem(current, current+1);

    reloadAction();

    selectRow(current + 1);
}

void ProfileTable::reloadAction()
{
    int count = builder->count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i);
    }
}
