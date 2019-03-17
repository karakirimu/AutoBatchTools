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
    this->header()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHeaderLabels((QStringList() << tr("Name") << tr("Version") \
                                            << tr("Vendor") << tr("Path")));

    builder = new ExtrafunctionsXmlBuilder();

    //row select action
    connect(this, &QAbstractItemView::clicked, this, &PluginsTree::itemSelectUpdate);
    connect(this->model(), &QAbstractItemModel::dataChanged, this, &PluginsTree::itemDataUpdate);
}

PluginsTree::~PluginsTree()
{
    delete builder;
}

void PluginsTree::reloadAction()
{
    ignoreDataChangedSignal(true);

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
    qDebug() << "[VariantTree::reloadAction] elapsed: " << time.elapsed() << "ms";
#endif

    ignoreDataChangedSignal(false);
}

void PluginsTree::addAction()
{

}

void PluginsTree::deleteAction()
{

}

void PluginsTree::upAction()
{

}

void PluginsTree::downAction()
{

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

bool PluginsTree::setManualPlugins(QTreeWidgetItem *parent)
{
    int count = builder->count();
    QList<QStringList> *list = new QList<QStringList>();

    for(int i = 0; i < count; i++){
        list->clear();
        if(builder->readItem(i, list)){
            QTreeWidgetItem *item = new QTreeWidgetItem(parent);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable \
                           | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);

            QPluginLoader loader(list->at(0).at(1));

            if(loader.load()){
                QObject *plugin = loader.instance();
                ExtraPluginInterface *inter = qobject_cast<ExtraPluginInterface *>(plugin);

                QString desc = inter->tooltipString();

                if(desc != ""){
                    item->setToolTip(0, desc);
                }

                //name
                QFileInfo info(list->at(0).at(1));
                item->setData(0, Qt::DisplayRole, QVariant(info.baseName()));

                //version
                item->setData(1, Qt::DisplayRole, QVariant(inter->version()));

                //vendor
                item->setData(2, Qt::DisplayRole, QVariant(inter->vendor()));

                //path
                item->setData(3, Qt::DisplayRole, QVariant(list->at(0).at(1)));

                loader.unload();

            }else{
                //name
                item->setData(0, Qt::DisplayRole, QVariant(tr("File is invalid.")));

                //path
                item->setData(3, Qt::DisplayRole, QVariant(list->at(0).at(1)));
            }
        }
    }

    delete list;
    return true;
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

void PluginsTree::ignoreDataChangedSignal(bool valid)
{
    if(valid){
        disconnect(this->model(), &QAbstractItemModel::dataChanged, this, &PluginsTree::itemDataUpdate);
    }else{
        connect(this->model(), &QAbstractItemModel::dataChanged, this, &PluginsTree::itemDataUpdate);
    }
}
