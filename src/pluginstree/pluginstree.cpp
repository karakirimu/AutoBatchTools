#include "pluginstree.h"

PluginsTree::PluginsTree(QWidget *parent)
    :QTreeWidget (parent)
{
    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &PluginsTree::onCustomContextMenu);

    //init menu context
    contextMenu = new QMenu(this);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //set header
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setHeaderLabels((QStringList() << tr("Name") << tr("Version") \
                                            << tr("Author") << tr("Path")));

    builder = new PluginsXmlBuilder();

    //init tree
    reloadAction();

    //row select action
    connect(this, &QAbstractItemView::clicked, this, &PluginsTree::itemSelectUpdate);
}

PluginsTree::~PluginsTree()
{
    delete builder;
}

void PluginsTree::reloadAction()
{
#ifdef QT_DEBUG
    QTime time;
    time.start();
#endif
    this->clear();

    //set treeitem
    QTreeWidgetItem *root_auto = new QTreeWidgetItem(this);
    root_auto->setData(0, Qt::DisplayRole, QVariant(tr("Auto-Detected")));
    root_auto->setExpanded(true);

    QTreeWidgetItem *root_manual = new QTreeWidgetItem(this);
    root_manual->setData(0, Qt::DisplayRole, QVariant(tr("Manual")));
    root_manual->setExpanded(true);

    setAutoDetectPlugins(root_auto);
    setManualPlugins(root_manual);

#ifdef QT_DEBUG
    qDebug() << "[PluginsTree::reloadAction] elapsed: " << time.elapsed() << "ms";
#endif
}

void PluginsTree::addAction()
{
    QModelIndex index = getSectionFromUi();
    QTreeWidgetItem *top = itemFromIndex(index);
    int rootindex = index.row();

    if(rootindex == TREE_MANUAL){
        int count = top->childCount();
        QList<QStringList> tlist;
        QStringList tmp;
        // open files
        QStringList strlist = selectFiles(QDir::currentPath());

        for (int i = 0; i < strlist.count(); i++) {
            tlist.clear();
            tmp.clear();
            tmp = loadPluginUiText(&(QStringList() << "" << "" << "" << strlist.at(i)));
            tlist = createXmlVariants(&tmp);

            builder->addItem(&tlist);
            this->insertRow(count + i, &tmp);
        }
    }
}

void PluginsTree::deleteAction()
{
    // selecteditems is const
    QList<QTreeWidgetItem *> top = this->selectedItems();

    //check delete warning message
    if(top.count() == 0 || !deleteCheckMessage()) return;

    // ItemWidget based delete
    QTreeWidgetItem *tmp;
    QMutableListIterator<QTreeWidgetItem *> i(top);

    while(i.hasNext()){
        tmp = i.next();
        // Extract only child items
        if (indexFromItem(tmp).parent().isValid()
                && indexFromItem(tmp).parent().row() == TREE_MANUAL){
            builder->deleteItem(indexFromItem(tmp).row());
            tmp->parent()->removeChild(tmp);

        }
    }
}

void PluginsTree::upAction()
{
    if(!isParentValid()) return;

    QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());

    int cur = this->currentIndex().row();

    if(cur == 0) return;
    swapRow(cur, cur - 1);
    this->selectionModel()->setCurrentIndex(indexFromItem(top->child(cur - 1)),\
                                   QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    builder->swapItem(cur, cur - 1);
}

void PluginsTree::downAction()
{
    if(!isParentValid()) return;

    QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());

    int count = top->childCount();
    int cur = this->currentIndex().row();

    if((cur + 1) == count) return;
    swapRow(cur, cur + 1);
    this->selectionModel()->setCurrentIndex(indexFromItem(top->child(cur + 1)),\
                                   QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    builder->swapItem(cur, cur + 1);
}

void PluginsTree::itemSelectUpdate()
{
    int row = this->selectionModel()->currentIndex().row();
    int column = this->selectionModel()->currentIndex().column();

    if (this->selectionModel()->currentIndex().parent().isValid()){
        qDebug() << QString("[PluginsTree::itemSelectUpdate] Position: (%1,%2)").arg(row).arg(column);
        QTreeWidgetItem *item = this->currentItem();
        item->setToolTip(column, item->data(column, Qt::DisplayRole).toString());

    }else{
        qDebug() << QString("[PluginsTree::itemSelectUpdate] Position: (%1,%2) in top level").arg(row).arg(column);
    }
}

void PluginsTree::insertRow(int row, QStringList *data)
{
    qDebug() << "[PluginsTree::insertRow] row : " << row;

    QModelIndex index = getSectionFromUi();
    this->insertRow(row, data, &index);
}

void PluginsTree::insertRow(int row, QStringList *data, QModelIndex *index)
{
    QAbstractItemModel *model = this->model();
    if (!model->insertRow(row, *index))
        return;

    closeEditState();

    for (int column = 0; column < model->columnCount(*index); ++column) {
        QModelIndex child = model->index(row, column, *index);
        model->setData(child, QVariant(data->at(column)), Qt::EditRole);
        this->itemFromIndex(child)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                                           | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
    }
}

void PluginsTree::replaceItem(int row, QStringList strlist, int section)
{
    QModelIndex index = this->indexFromItem(this->topLevelItem(section));
    QAbstractItemModel *model = this->model();

    QTreeWidgetItem *child = itemFromIndex(index)->child(row);

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        child->setData(column, Qt::EditRole, QVariant(strlist.at(column)));
//        child->setToolTip(column, strlist.at(column));
    }

}

void PluginsTree::swapRow(int before, int after)
{
    qDebug() << "[PluginsTree::swapRow(UI)] before : " << before << " after : " << after;

    QModelIndex index = getSectionFromUi();
    this->swapRow(before, after, &index);
}

void PluginsTree::swapRow(int before, int after, QModelIndex *index)
{
    //if the point of distination is parent or not.
    QVariantList bku;
    bku.append(index->sibling(before, 0).data());
    bku.append(index->sibling(before, 1).data());
    QAbstractItemModel *model = this->model();

    //set item
    for (int column = 0; column < model->columnCount(*index); ++column) {
        QModelIndex child = index->sibling(before, column);
        model->setData(child, QVariant(index->sibling(after, column).data()), Qt::EditRole);

        child = index->sibling(after, column);
        model->setData(child, QVariant(bku.at(column)), Qt::EditRole);
    }
}

void PluginsTree::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add File(s)"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &PluginsTree::addAction);
    connect(m_delete, &QAction::triggered, this, &PluginsTree::deleteAction);
}

void PluginsTree::setPopupActionDefault()
{
    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    //connect signals
    connect(m_up, &QAction::triggered, this, &PluginsTree::upAction);
    connect(m_down, &QAction::triggered, this, &PluginsTree::downAction);
}

void PluginsTree::setPopupActionBottom()
{
    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_ref, &QAction::triggered, this, &PluginsTree::reloadAction);
}

bool PluginsTree::eventFilter(QObject *obj, QEvent *event)
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
                 QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());
                 int current = this->currentIndex().row();

                 if(current != 0)
                     this->selectionModel()->setCurrentIndex(indexFromItem(top->child(current - 1)) \
                                     , QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

                 closeEditState();
             }

             break;

           case Qt::Key_Down:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 downAction();
             }else{
                 QTreeWidgetItem *top = itemFromIndex(getSectionFromUi());
                 int count = top->childCount();
                 int current = this->currentIndex().row();

                 if(count - 1 != current)
                     this->selectionModel()->setCurrentIndex(indexFromItem(top->child(current + 1)) \
                                     , QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

                 closeEditState();
             }
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

void PluginsTree::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

bool PluginsTree::setAutoDetectPlugins(QTreeWidgetItem *parent)
{
    // file search settings
    QList<QStringList> searchitem;
    searchitem.append(QStringList() << "name" << "autoplug");
    searchitem.append(QStringList() << "keyword" << "*.dll");
    searchitem.append(QStringList() << "dir" << "./plugins");

    QStringList list;

    //file search
    FileSearchLoader fsload;
    list = fsload.searchFromStrList(&searchitem);

    int count = list.count();

    for(int i = 0; i < count; i++){
        QTreeWidgetItem *item = new QTreeWidgetItem(parent);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                       | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);

        QStringList data = loadPluginUiText(&(QStringList() << "" << "" << "" << list.at(i)));

        if(data.at(DESC_UI) != ""){
            item->setToolTip(0, data.at(DESC_UI));
        }

        //name
        item->setData(0, Qt::DisplayRole, QVariant(data.at(NAME_UI)));

        //version
        item->setData(1, Qt::DisplayRole, QVariant(data.at(VERSION_UI)));

        //vendor
        item->setData(2, Qt::DisplayRole, QVariant(data.at(VENDOR_UI)));

        //path
        item->setData(3, Qt::DisplayRole, QVariant(data.at(PATH_UI)));

    }

    return true;
}

bool PluginsTree::setManualPlugins(QTreeWidgetItem *parent)
{
    int count = builder->count();
    QList<QStringList> list;

    for(int i = 0; i < count; i++){
        list.clear();
        if(builder->readItem(i, &list)){
            QTreeWidgetItem *item = new QTreeWidgetItem(parent);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                           | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);

            QStringList data = loadPluginUiText(&(list.first()));

            if(data.at(DESC_UI) != ""){
                item->setToolTip(0, data.at(DESC_UI));
            }

            //name
            item->setData(0, Qt::DisplayRole, QVariant(data.at(NAME_UI)));

            //version
            item->setData(1, Qt::DisplayRole, QVariant(data.at(VERSION_UI)));

            //vendor
            item->setData(2, Qt::DisplayRole, QVariant(data.at(VENDOR_UI)));

            //path
            item->setData(3, Qt::DisplayRole, QVariant(data.at(PATH_UI)));

        }else{
            return false;

        }
    }

    return true;
}

QStringList PluginsTree::getEmptyVariants()
{
    return QStringList() << builder->PL_NAME << "" << builder->PL_ATTR_FILE << "";
}

QStringList PluginsTree::getVariants(QTreeWidgetItem *childitem)
{
    QStringList list = getEmptyVariants();
    QModelIndex child = indexFromItem(childitem);

    if(child.parent().isValid()){
        int row = child.row();
        list.replace(1, child.parent().sibling(row, 0).data().toString());
        list.replace(3, child.parent().sibling(row, 1).data().toString());

    }

    return list;
}

QList<QStringList> PluginsTree::createXmlVariants(QStringList *uitext)
{
    QList<QStringList> listdata;
    listdata.append(QStringList() << builder->PL_NAME << uitext->at(NAME_UI) \
                                << builder->PL_ATTR_FILE << uitext->at(PATH_UI));
    return listdata;
}

//todo: name
QStringList PluginsTree::loadPluginUiText(const QStringList *xmltext)
{
    QPluginLoader loader(xmltext->at(PATH_XML));
    QFileInfo info(xmltext->at(PATH_XML));

    QStringList hlist;

    if(loader.load()){
        QObject *plugin = loader.instance();
        ExtraPluginInterface *inter = qobject_cast<ExtraPluginInterface *>(plugin);

        PLUGININFO pinfo = inter->pluginInfo();

        hlist << pinfo.name << pinfo.version
              << pinfo.author << xmltext->at(PATH_XML) << pinfo.tooltip;

        loader.unload();

    }else{
        hlist << info.baseName() << "" << "" << xmltext->at(PATH_XML) << "";
    }

    return hlist;
}

bool PluginsTree::isPluginValid(const QStringList *xmltext)
{
    QPluginLoader loader(xmltext->at(PATH_XML));

    if(loader.load()){
        loader.unload();
        return true;
    }

    return false;
}

void PluginsTree::closeEditState()
{
    if (this->selectionModel()->currentIndex().isValid()) {
        this->closePersistentEditor(this->currentItem());
    }
}

bool PluginsTree::isParentValid()
{
    if(this->selectionModel()->selection().isEmpty()
            || !this->selectionModel()->currentIndex().parent().isValid()) return false;

    return true;
}

QModelIndex PluginsTree::getSectionFromUi()
{
    if (this->selectionModel()->currentIndex().parent().isValid()){
        return this->currentIndex().parent();
    }else{
        return this->currentIndex();
    }
}

bool PluginsTree::deleteCheckMessage()
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

QStringList PluginsTree::selectFiles(QString basedir)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    return dialog.getOpenFileNames(this, tr("Open Plugins"), basedir, tr("Files (*.dll)"));
}
