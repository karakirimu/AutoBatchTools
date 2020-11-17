/*
 * Copyright 2016-2019 karakirimu
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

#include "stringtable.h"

StringTable::StringTable(QWidget *parent) : BasicTable(parent)
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
    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Global Variant") << tr("Value")));

    //set vertical header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                    , qpc.VERTICAL_HEADER_ENABLE);

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
    m_add = addTableAction(ACTION::ADD, Qt::CTRL + Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL + Qt::Key_E);
    contextMenu->addSeparator();

    m_file = addTableAction(ACTION::FILE);
    m_dir = addTableAction(ACTION::FOLDER);
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &StringTable::addAction);
    connect(m_delete, &QAction::triggered, this, &StringTable::deleteAction);
    connect(m_edit, &QAction::triggered, this, &StringTable::editAction);
    connect(m_file, &QAction::triggered, this, &StringTable::openFileAction);
    connect(m_dir, &QAction::triggered, this, &StringTable::openDirectoryAction);
}

void StringTable::setPopupActionDefault()
{
    //set basic items
    m_cut = addTableAction(ACTION::CUT, Qt::CTRL + Qt::Key_X);
    m_copy = addTableAction(ACTION::COPY, Qt::CTRL + Qt::Key_C);
    m_paste = addTableAction(ACTION::PASTE, Qt::CTRL + Qt::Key_V);

    contextMenu->addSeparator();

    m_up = addTableAction(ACTION::UP, Qt::CTRL + Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL + Qt::Key_Down);

    //connect signals
    connect(m_cut, &QAction::triggered, this, &StringTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &StringTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &StringTable::pasteAction);
    connect(m_up, &QAction::triggered, this, &StringTable::upAction);
    connect(m_down, &QAction::triggered, this, &StringTable::downAction);
}

void StringTable::setPopupActionBottom()
{
    contextMenu->addSeparator();

    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL + Qt::Key_R);

    connect(m_ref, &QAction::triggered, this, &StringTable::reloadAction);
}

bool StringTable::eventFilter(QObject *obj, QEvent *event)
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

           case Qt::Key_X:      if (mdCheck())  cutAction();     break;
           case Qt::Key_C:      if (mdCheck())  copyAction();    break;
           case Qt::Key_V:      if (mdCheck())  pasteAction();   break;
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

void StringTable::createList(int row, QList<QStringList> *newlist)
{
    //add variant
    newlist->append(QStringList() << "variant" << this->model()->index(row, 0).data().toString());
    //add value
    newlist->append(QStringList() << "value" << this->model()->index(row, 1).data().toString());
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
        QModelIndexList lists = this->selectedIndexes();
        while(!lists.empty()){
            this->removeRow(lists.at(0).row());
            builder->deleteItem(lists.at(0).row());

            //delete column index
            for(int i = 0; i < lists.at(0).column(); i++){
                lists.removeAt(0);
            }
            lists = this->selectedIndexes();
        }
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

//TODO: for fast
void StringTable::cutAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    copyAction();

    // modified delete table recursive
    QModelIndexList lists = this->selectedIndexes();
    while(!lists.empty()){
        this->removeRow(lists.at(0).row());
        builder->deleteItem(lists.at(0).row());

        //delete column index
        for(int i = 0; i < lists.at(0).column(); i++){
            lists.removeAt(0);
        }
        lists = this->selectedIndexes();
    }
}

void StringTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    // copy from VariantTable::copyAction()
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
}

void StringTable::pasteAction()
{
    //copy from CommandTable::pasteEnterAction()
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(QRegularExpression("\\n|\\r\\n"));

    //last lests unknown ""
    if(text.count() > 1) text.removeLast();

    int row = this->rowCount();
    int txcount = text.count();

    for(int i = 0; i < txcount; i++){
       if(row > 0) row = this->currentRow();
       insertRow(row);

       // One row table
       QStringList intext = (text.at(i)).split(QRegularExpression("\\t|,"));

       int intxt = intext.count();
       if(intxt > 0){
           this->setItem(row, 0, new QTableWidgetItem(intext.at(0)));
           if(intxt > 1){
               this->setItem(row, 1, new QTableWidgetItem(intext.at(1)));
           }
       }

       //update
       resave();
    }
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
    QString str = selectFile(QDir::currentPath());
    this->setItem(current, 1, new QTableWidgetItem(str));
}

void StringTable::openDirectoryAction()
{
    int current = this->currentRow();
    if(rowCount() == 0 || current < 0) return;
    QString str = selectFolder(QDir::currentPath());
    this->setItem(current, 1, new QTableWidgetItem(str));
}

void StringTable::saveAction(int row)
{
    //set item
    QList<QStringList> list;
    createList(row, &list);
    builder->editItem(row, &list);
}

//FIXME: not efficient method
void StringTable::resave()
{
    //clear item
    int datacount = builder->count();
    for(int i = 0; i < datacount; i++) builder->deleteItem(0);

    datacount = this->rowCount();

    for(int i = 0; i < datacount; i++){
        //add empty item
        QList<QStringList> list;
        createList(i, &list);
        builder->addItem(&list);
        //oversave item
        saveAction(i);
    }
}
