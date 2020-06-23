/*
 * Copyright 2016-2020 karakirimu
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

#include "varianttree.h"

VariantTree::VariantTree(QWidget *)
{
    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &VariantTree::onCustomContextMenu);

    //init menu context
    contextMenu = new QMenu(this);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //set header
    this->header()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHeaderLabels((QStringList() << tr("Variant") << tr("Value")));

    //set new xml builder
    builder = new StringXmlBuilder();

    //row select action
    connect(this, &QAbstractItemView::clicked, this, &VariantTree::itemSelectUpdate);
    connect(this->model(), &QAbstractItemModel::dataChanged, this, &VariantTree::itemDataUpdate);
}

VariantTree::~VariantTree()
{
    delete builder;
    delete contextMenu;
}

void VariantTree::setEditOperator(EditOperator *op)
{
    editop = op;
}

void VariantTree::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");
    if(sep.count() < 2) return;

    //get operation code
    int command = sep.last().toInt();
    int rowpos = -1;
    int treerowpos = -1;

    switch (command) {
    case UiCommandMap::GV_ADD_TREE:
        this->insertRow(static_cast<QString>(sep.at(0)).toInt(), TREE_GLOBAL);
        break;

    case UiCommandMap::GV_DEL_TREE:
        this->deleteRow(static_cast<QString>(sep.at(0)).toInt(), TREE_GLOBAL);
        break;

    case UiCommandMap::GV_EDIT_TREE:
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        this->replaceItem(rowpos, getGlobalRowElement(rowpos), TREE_GLOBAL);
        break;

    case UiCommandMap::GV_INS_TREE:
        treerowpos = static_cast<QString>(sep.at(0)).toInt();
        this->insertRow(treerowpos, TREE_GLOBAL);
        this->replaceItem(treerowpos, getGlobalRowElement(treerowpos), TREE_GLOBAL);
        break;

    case UiCommandMap::GV_SWAP_TREE:
        this->swapRow(static_cast<QString>(sep.at(0)).toInt()
                          , static_cast<QString>(sep.at(1)).toInt(), TREE_GLOBAL);
        break;

    case UiCommandMap::LV_ADD:
        this->insertRow(static_cast<QString>(sep.at(0)).toInt(), TREE_LOCAL);
        break;

    case UiCommandMap::LV_DELETE:
        this->deleteRow(static_cast<QString>(sep.at(0)).toInt(), TREE_LOCAL);
        break;

    case UiCommandMap::LV_EDIT:
        rowpos = static_cast<QString>(sep.at(1)).toInt();
        this->replaceItem(rowpos, getLocalRowElement(LOCALINDEX, rowpos), TREE_LOCAL);
        break;

    case UiCommandMap::LV_INSERT:
        treerowpos = static_cast<QString>(sep.at(1)).toInt();
        this->insertRow(treerowpos, TREE_LOCAL);
        this->replaceItem(treerowpos, getLocalRowElement(LOCALINDEX, treerowpos), TREE_LOCAL);
        break;

    case UiCommandMap::LV_SWAP:
        this->swapRow(static_cast<QString>(sep.at(0)).toInt()
                          , static_cast<QString>(sep.at(1)).toInt(), TREE_LOCAL);
        break;

    default:
        break;
    }
}

void VariantTree::reloadAction()
{
    ignoreDataChangedSignal(true);

#ifdef QT_DEBUG
    QElapsedTimer time;
    time.start();
#endif    
    this->clear();

    //set treeitem
    QTreeWidgetItem *root_global = new QTreeWidgetItem(this);
    root_global->setData(0, Qt::DisplayRole, QVariant(tr("Global")));
    root_global->setExpanded(true);

    QTreeWidgetItem *root_local = new QTreeWidgetItem(this);
    root_local->setData(0, Qt::DisplayRole, QVariant(tr("Local")));
    root_local->setExpanded(true);

//    QTreeWidgetItem *root_internal = new QTreeWidgetItem(this);
//    root_internal->setData(0, Qt::DisplayRole, QVariant(tr("Internal")));

    setGlobalListItems(root_global);
    setLocalListItems(root_local, LOCALINDEX);


#ifdef QT_DEBUG
    qDebug() << "[VariantTree::reloadAction] elapsed: " << time.elapsed() << "ms";
#endif

    ignoreDataChangedSignal(false);
}

//tested
void VariantTree::addAction()
{
    ignoreDataChangedSignal(true);

    QModelIndex index = getSectionFromUi();
    QTreeWidgetItem *top = itemFromIndex(index);
    int rootindex = index.row();
    int count = top->childCount();

    if(rootindex == TREE_GLOBAL){
        editop->treeEditGVariantAction(count, getEmptyVariants() \
                                       , UiCommandMap::GV_ADD_TREE);
    }else if(rootindex == TREE_LOCAL){
        editop->treeEditLVariantAction(count, getEmptyVariants() \
                                       , UiCommandMap::LV_ADD, LOCALINDEX);
    }

    this->insertRow(count);
    ignoreDataChangedSignal(false);
}

//tested
void VariantTree::editAction()
{
    this->edit(currentIndex());
}


//tested single/multiple row
void VariantTree::deleteAction()
{
    ignoreDataChangedSignal(true);

    //check delete warning message
    if(!deleteCheckMessage()) return;

    // selecteditems is const
    QList<QTreeWidgetItem *> top = this->selectedItems();

    // ItemWidget based delete
    QTreeWidgetItem *tmp;
    QMutableListIterator<QTreeWidgetItem *> i(top);

    int rootindex = 0;
    while(i.hasNext()){
        tmp = i.next();
        // Extract only child items
        if (indexFromItem(tmp).parent().isValid()){
            rootindex = indexFromItem(tmp).parent().row();

            if(rootindex == TREE_GLOBAL){
                editop->treeEditGVariantAction(indexFromItem(tmp).row() \
                        , getVariants(tmp), UiCommandMap::GV_DEL_TREE);
            }else if(rootindex == TREE_LOCAL){
                editop->treeEditLVariantAction(indexFromItem(tmp).row() \
                        , getVariants(tmp), UiCommandMap::LV_DELETE, LOCALINDEX);
            }

            tmp->parent()->removeChild(tmp);
        }
    }

    ignoreDataChangedSignal(false);
}


//tested
void VariantTree::cutAction()
{
    copyAction();

    ignoreDataChangedSignal(true);

    // selecteditems is const
    QList<QTreeWidgetItem *> top = this->selectedItems();

    // ItemWidget based delete
    QTreeWidgetItem *tmp;
    QMutableListIterator<QTreeWidgetItem *> i(top);

    int rootindex = 0;
    while(i.hasNext()){
        tmp = i.next();
        // Extract only child items
        if (indexFromItem(tmp).parent().isValid()){
            rootindex = indexFromItem(tmp).parent().row();

            if(rootindex == TREE_GLOBAL){
                editop->treeEditGVariantAction(indexFromItem(tmp).row() \
                        , getVariants(tmp), UiCommandMap::GV_DEL_TREE);
            }else if(rootindex == TREE_LOCAL){
                editop->treeEditLVariantAction(indexFromItem(tmp).row() \
                        , getVariants(tmp), UiCommandMap::LV_DELETE, LOCALINDEX);
            }

            tmp->parent()->removeChild(tmp);
        }
    }

    ignoreDataChangedSignal(false);
}

//tested single row copy (value1    value2  ...)
//tested multiple row copy
void VariantTree::copyAction()
{
    if(!isParentValid()) return;

    // copy from VariantTable::copyAction()
    QString tmp;
    QModelIndexList mlist = this->selectedIndexes();

    // 2 column
    int counter = mlist.count();
    for(int i = 0; i < counter; i++){
        int crow = mlist.at(i).row();
        tmp.append(this->selectionModel()->currentIndex().\
                   sibling(crow, i % 2).data().toString());

        tmp.append((i%2 == 0)? "\t" : "\n");

    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmp);

}

//tested single row text & single text
void VariantTree::pasteAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    QStringList text = clipboard->text().split(QRegularExpression("\\n|\\r\\n"), Qt::SkipEmptyParts);
    QModelIndex index = getSectionFromUi();

    int row = 0;
    int txcount = text.count();

    ignoreDataChangedSignal(true);

    for(int i = 0; i < txcount; i++){

       if (this->selectionModel()->currentIndex().parent().isValid()){
           row = this->currentIndex().row() + 1;
       }else{
           row = itemFromIndex(index)->childCount();
       }

       int rootindex = index.row();
       insertRow(row, rootindex);

       // One row table or single text
       QStringList intext = (text.at(i)).split(QRegularExpression("\\t|,"));

       // when you want to put filename or variant selected column.
       if(intext.count() == 1){
            int column = this->selectionModel()->currentIndex().column();
            intext.insert(1 - column, "");
       }

       this->replaceItem(row, intext, rootindex);

       //insert
       if(rootindex == TREE_GLOBAL){
           editop->treeEditGVariantAction(row \
                   , intext, UiCommandMap::GV_INS_TREE);
       }else if(rootindex == TREE_LOCAL){
           editop->treeEditLVariantAction(row \
                   , intext, UiCommandMap::LV_INSERT, LOCALINDEX);
       }
    }

    ignoreDataChangedSignal(false);
}

// tested
void VariantTree::openFileAction()
{
    if(!isParentValid()) return;

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString str = dialog.getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("File (*.*)"));

    int current = this->currentIndex().row();
    QModelIndex child = this->model()->index(current, 1, this->selectionModel()->currentIndex().parent());
    this->model()->setData(child, QVariant(str), Qt::EditRole);
}

// tested
void VariantTree::openDirAction()
{
    if(!isParentValid()) return;

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString str = dialog.getExistingDirectory(this, tr("Open Folder"), QDir::currentPath());

    int current = this->currentIndex().row();
    QModelIndex child = this->model()->index(current, 1, this->selectionModel()->currentIndex().parent());
    this->model()->setData(child, QVariant(str), Qt::EditRole);
}

//tested
void VariantTree::upAction()
{
    if(!isParentValid()) return;

    ignoreDataChangedSignal(true);

    QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());

    int cur = this->currentIndex().row();

    if(cur == 0) return;
    swapRow(cur, cur - 1);
    this->selectionModel()->setCurrentIndex(indexFromItem(top->child(cur - 1)),\
                                   QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    int rootindex = this->currentIndex().parent().row();

    if(rootindex == TREE_GLOBAL){
        editop->treeSwapGVariantAction(cur, cur - 1);
    }else if(rootindex == TREE_LOCAL){
        editop->treeSwapLVariantAction(cur, cur - 1, LOCALINDEX);
    }

    ignoreDataChangedSignal(false);

}

//tested
void VariantTree::downAction()
{
    if(!isParentValid()) return;

    ignoreDataChangedSignal(true);

    QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());

    int count = top->childCount();
    int cur = this->currentIndex().row();

    if((cur + 1) == count) return;
    swapRow(cur, cur + 1);
    this->selectionModel()->setCurrentIndex(indexFromItem(top->child(cur + 1)),\
                                   QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    int rootindex = this->currentIndex().parent().row();

    if(rootindex == TREE_GLOBAL){
        editop->treeSwapGVariantAction(cur, cur + 1);
    }else if(rootindex == TREE_LOCAL){
        editop->treeSwapLVariantAction(cur, cur + 1, LOCALINDEX);
    }

    ignoreDataChangedSignal(false);
}


void VariantTree::itemSelectUpdate()
{
    int row = this->selectionModel()->currentIndex().row();
    int column = this->selectionModel()->currentIndex().column();

    if (this->selectionModel()->currentIndex().parent().isValid()){
        qDebug() << QString("[VariantTree::itemSelectUpdate] Position: (%1,%2)").arg(row).arg(column);
        QTreeWidgetItem *item = this->currentItem();
        item->setToolTip(column, item->data(column, Qt::DisplayRole).toString());

    }else{
        qDebug() << QString("[VariantTree::itemSelectUpdate] Position: (%1,%2) in top level").arg(row).arg(column);
    }
}

//tested
void VariantTree::itemDataUpdate(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    qDebug() << "[VariantTree::itemDataUpdate] currentindex : " \
             << this->currentIndex().data().toString();

    int rootindex = this->currentIndex().parent().row();
    if(rootindex == TREE_GLOBAL){
        editop->treeEditGVariantAction(this->currentIndex().row() \
                , getVariants(this->currentItem()), UiCommandMap::GV_EDIT_TREE);
    }else if(rootindex == TREE_LOCAL){
        editop->treeEditLVariantAction(this->currentIndex().row() \
                , getVariants(this->currentItem()), UiCommandMap::LV_EDIT, LOCALINDEX);
    }

}

void VariantTree::insertRow(int row)
{
    qDebug() << "[VariantTree::insertRow] row : " << row;

    QModelIndex index = getSectionFromUi();
    this->insertRow(row, &index);
}

void VariantTree::insertRow(int row, int section)
{
    qDebug() << "[VariantTree::insertRow] row : " << row;

    QModelIndex index = indexFromItem(this->topLevelItem(section));
    this->insertRow(row, &index);
}

void VariantTree::insertRow(int row, QModelIndex *index)
{
    ignoreDataChangedSignal(true);

    QAbstractItemModel *model = this->model();
    if (!model->insertRow(row, *index))
        return;

    closeEditState();

    QStringList data = getEmptyVariants();

    for (int column = 0; column < model->columnCount(*index); ++column) {
        QModelIndex child = model->index(row, column, *index);
        model->setData(child, QVariant(data.at(column)), Qt::EditRole);
        this->itemFromIndex(child)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                                           | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    }

    ignoreDataChangedSignal(false);
}

void VariantTree::deleteRow(int row, int section)
{
    QModelIndex index = this->indexFromItem(this->topLevelItem(section));
    QAbstractItemModel *model = this->model();
    if (model->removeRow(row, index))
        closeEditState();
}

// tested(only UI) (only used in Ui)
void VariantTree::deleteRowItems()
{
    // selecteditems is const
    QList<QTreeWidgetItem *> top = this->selectedItems();

    // ItemWidget based delete
    QTreeWidgetItem *tmp;
    QMutableListIterator<QTreeWidgetItem *> i(top);
    while(i.hasNext()){
        tmp = i.next();
        // Extract only child items
        if (indexFromItem(tmp).parent().isValid()){
            tmp->parent()->removeChild(tmp);
        }
    }
}

void VariantTree::replaceItem(int row, QStringList strlist, int section)
{
    ignoreDataChangedSignal(true);

    QModelIndex index = this->indexFromItem(this->topLevelItem(section));
    QAbstractItemModel *model = this->model();

    QTreeWidgetItem *child = itemFromIndex(index)->child(row);

    if(strlist.count() == 0){
        ignoreDataChangedSignal(false);
        return;

    }else{
        for (int column = 0; column < model->columnCount(index.parent()); ++column) {
            child->setData(column, Qt::EditRole, QVariant(strlist.at(column)));
        }
    }

    ignoreDataChangedSignal(false);
}

void VariantTree::swapRow(int before, int after)
{
    qDebug() << "[VariantTree::swapRow(UI)] before : " << before << " after : " << after;

    QModelIndex index = getSectionFromUi();
    this->swapRow(before, after, &index);
}

void VariantTree::swapRow(int before, int after, int section)
{
    qDebug() << "[VariantTree::swapRow(Undo/Redo)] before : " << before << " after : " << after;

    QModelIndex index = indexFromItem(this->topLevelItem(section));
    this->swapRow(before, after, &index);
}

void VariantTree::swapRow(int before, int after, QModelIndex *index)
{
    //if the point of distination is parent or not.
    QVariantList bku;

    bku.append(this->model()->index(before, 0, *index).data());
    bku.append(this->model()->index(before, 1, *index).data());
    QAbstractItemModel *model = this->model();

    //set item
    for (int column = 0; column < model->columnCount(*index); ++column) {
        QModelIndex child = this->model()->index(before, column, *index);
        model->setData(child, QVariant(this->model()->index(after, column, *index).data()), Qt::EditRole);

        child = this->model()->index(after, column, *index);
        model->setData(child, QVariant(bku.at(column)), Qt::EditRole);
    }
}

void VariantTree::setPopupActionTop()
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
    connect(m_add, &QAction::triggered, this, &VariantTree::addAction);
    connect(m_delete, &QAction::triggered, this, &VariantTree::deleteAction);
    connect(m_edit, &QAction::triggered, this, &VariantTree::editAction);

    connect(m_file, &QAction::triggered, this, &VariantTree::openFileAction);
    connect(m_dir, &QAction::triggered, this, &VariantTree::openDirAction);

}

void VariantTree::setPopupActionDefault()
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
    connect(m_cut, &QAction::triggered, this, &VariantTree::cutAction);
    connect(m_copy, &QAction::triggered, this, &VariantTree::copyAction);
    connect(m_paste, &QAction::triggered, this, &VariantTree::pasteAction);
    connect(m_up, &QAction::triggered, this, &VariantTree::upAction);
    connect(m_down, &QAction::triggered, this, &VariantTree::downAction);
}

void VariantTree::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, &QAction::triggered, this, &VariantTree::reloadAction);
}

bool VariantTree::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

    auto mdCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::ControlModifier);
    };

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:  if (mdCheck()) addAction();    break;
        case Qt::Key_Delete: if (mdCheck()) deleteAction(); break;
        case Qt::Key_E:      if (mdCheck()) editAction();   break;

        case Qt::Key_Up:
            if(mdCheck()){
                upAction();
            }else{
                int current = this->currentIndex().row();
                if(current != 0)
                    this->selectionModel()->setCurrentIndex(this->model()->index(current - 1, 0, this->currentIndex().parent()) \
                                     , QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

                closeEditState();
            }
            break;

        case Qt::Key_Down:
            if(mdCheck()){
                downAction();
            }else{
                QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());
                int count = top->childCount();
                int current = this->currentIndex().row();

                if(count - 1 != current)
                    this->selectionModel()->setCurrentIndex(this->model()->index(current + 1, 0, this->currentIndex().parent()) \
                                     , QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

                closeEditState();
            }
            break;

        case Qt::Key_X:  if(mdCheck()) cutAction();    break;
        case Qt::Key_C:  if(mdCheck()) copyAction();   break;
        case Qt::Key_V:  if(mdCheck()) pasteAction();  break;
        case Qt::Key_R:  if(mdCheck()) reloadAction(); break;
        default:
            //qDebug("Ate key press %d", keyEvent->key());
            break;
        }
        return true;
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}

void VariantTree::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

//todo: moveto last index
void VariantTree::dropEvent(QDropEvent *event)
{
    QModelIndex droppedIndex = indexAt( event->pos() );

    if( droppedIndex.isValid() && droppedIndex.parent().isValid()){
        //item moving (to xml) (cut + paste)
        QModelIndexList mlist = qobject_cast<VariantTree *>(event->source())->selectionModel()->selectedRows();

        //source index check
        for(int i = 0; i < mlist.count(); i++){
            if(!mlist.at(i).parent().isValid()){
                event->setDropAction(Qt::IgnoreAction);
                return;
            }
        }

        //source delete
        int sourceindex = 0;
        int destindex = droppedIndex.parent().row();

        for(int i = 0; i < mlist.count(); i++){
            sourceindex = mlist.at(i).parent().row();

            if(sourceindex == TREE_GLOBAL){
                editop->treeEditGVariantAction(mlist.at(i).row() \
                        , getVariants(itemFromIndex(mlist.at(i))), UiCommandMap::GV_DEL_TREE);
            }else if(sourceindex == TREE_LOCAL){
                editop->treeEditLVariantAction(mlist.at(i).row() \
                        , getVariants(itemFromIndex(mlist.at(i))), UiCommandMap::GV_DEL_TREE, LOCALINDEX);
            }

            //dest insert
            if(destindex == TREE_GLOBAL){
                editop->treeEditGVariantAction(droppedIndex.row() + i \
                        , getVariants(itemFromIndex(mlist.at(i))), UiCommandMap::GV_INS_TREE);
            }else if(destindex == TREE_LOCAL){
                editop->treeEditLVariantAction(droppedIndex.row() + i \
                        , getVariants(itemFromIndex(mlist.at(i))), UiCommandMap::GV_INS_TREE, LOCALINDEX);
            }

            qDebug()<< "[VariantTree::dropEvent] source : " << mlist.at(i).row() \
                    << " dest : " << droppedIndex.row();
        }



    }else{
        event->setDropAction(Qt::IgnoreAction);
        return;
    }


    QTreeWidget::dropEvent(event);
}

bool VariantTree::setLocalListItems(QTreeWidgetItem *parent, int itemid)
{
    EditorCache list;

    if(!editop->read(itemid, &list)){
        return false;
    }

    FunctionType fs;

    //get type
    QString type = list.type;

    //set root
    if(fs.getType(list.type) != fs.TYPE::LOCAL){
        return false;
    }

    int counter = list.local.variantData.count();

    for(int i = 0; i < counter; i++){
        VariantPair pair = list.local.variantData.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(parent);

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                       | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);

        item->setData(0, Qt::EditRole, QVariant(pair.variant));
        item->setData(1, Qt::EditRole, QVariant(pair.value));
    }

    qDebug()<< "[VariantTree::setLocalListItem] success";
    return true;
}

// needs to update
bool VariantTree::setGlobalListItems(QTreeWidgetItem *parent)
{
    int count = builder->count();
    QList<QStringList> *list = new QList<QStringList>();

    for(int i = 0; i < count; i++){
        list->clear();
        if(builder->readItem(i, list)){
            QTreeWidgetItem *item = new QTreeWidgetItem(parent);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                           | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
            for (int column = 0; column < this->header()->count(); ++column) {
                item->setData(column, Qt::EditRole, QVariant(list->at(column).at(1)));
//                item->setToolTip(column, list->at(column).at(1));
            }
        }
    }

    delete list;
    return true;
}

QStringList VariantTree::getLocalRowElement(int targetrow, int tablerow)
{
    EditorCache list;

    if(targetrow != LOCALINDEX || !editop->read(targetrow, &list)){
        return QStringList() << "" << "";
    }

    VariantPair pair = list.local.variantData.at(tablerow);

    return QStringList() <<  pair.variant << pair.value;

}

QStringList VariantTree::getGlobalRowElement(int row)
{
    QList<QStringList> list;
    if(builder->readItem(row, &list)){
        return QStringList() << list.at(0).at(1) << list.at(1).at(1);
    }
    return QStringList() << "" << "";
}

QStringList VariantTree::getEmptyVariants()
{
    return QStringList() << "" << "";
}

QStringList VariantTree::getVariants(QTreeWidgetItem *childitem)
{
    QStringList list;
    QModelIndex child = indexFromItem(childitem);

    if(child.parent().isValid()){
        int row = child.row();

        list.append(this->model()->index(row, 0, child.parent()).data().toString());
        list.append(this->model()->index(row, 1, child.parent()).data().toString());

    }else{
        list.append("");
        list.append("");

    }

    return list;
}

//tested(only UI)
void VariantTree::closeEditState()
{
    if (this->selectionModel()->currentIndex().isValid()) {
        this->closePersistentEditor(this->currentItem());
    }
}

//tested(only UI)
bool VariantTree::isParentValid()
{
    if(this->selectionModel()->selection().isEmpty()
            || !this->selectionModel()->currentIndex().parent().isValid()) return false;

    return true;
}

//return parent (call from UI only) (global local inner)
QModelIndex VariantTree::getSectionFromUi()
{
    if (this->selectionModel()->currentIndex().parent().isValid()){
        return this->currentIndex().parent();
    }else{
        return this->currentIndex();
    }
}

bool VariantTree::deleteCheckMessage()
{
    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert"), tr("Do you want to delete selected variants ?"), QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

void VariantTree::ignoreDataChangedSignal(bool valid)
{
    if(valid){
        disconnect(this->model(), &QAbstractItemModel::dataChanged, this, &VariantTree::itemDataUpdate);
    }else{
        connect(this->model(), &QAbstractItemModel::dataChanged, this, &VariantTree::itemDataUpdate);
    }
}
