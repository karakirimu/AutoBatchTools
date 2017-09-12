#include "profiletreewidget.h"

#include <QDateTime>

ProfileTreeWidget::ProfileTreeWidget(QWidget *)
{
    //set header
    this->header()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHeaderLabels((QStringList() << tr("Resource") << tr("Type")));

    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &ProfileTreeWidget::onCustomContextMenu);

    //init menu context
    contextMenu = new QMenu(this);

    //set popup action -> move to set widgetssignalbinder
//    popupAction();

    //row select action
    connect(this, &QAbstractItemView::clicked, this, &ProfileTreeWidget::rowSelected);
}

ProfileTreeWidget::~ProfileTreeWidget()
{
    delete contextMenu;
}

int ProfileTreeWidget::fixedCurrentRow()
{
    int current = currentRow();
    return (current > 0)? current + 1 : 0;
}

void ProfileTreeWidget::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

bool ProfileTreeWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:
             if (keyEvent->modifiers() & Qt::ControlModifier)
//               addAction();
                binder->addItem();
             break;

           case Qt::Key_Delete:
             deleteAction();
             break;

           case Qt::Key_Up:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     this->setCurrentItem(this->topLevelItem(this->currentRow() - 1));
             }
             break;

           case Qt::Key_Down:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 downAction();
             }else{
                 if(this->currentRow() != 0)
                     this->setCurrentItem(this->topLevelItem(this->currentRow() + 1));
             }
            break;

           case Qt::Key_C:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                 copyAction();
             break;

           case Qt::Key_E:
             if (keyEvent->modifiers() & Qt::ControlModifier)
                //emit editActionKeyPressed();
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

//    qDebug() << "profiletreewidget :: event : " << event->type();
    // standard event processing
    return QObject::eventFilter(obj, event);
}

//void ProfileTreeWidget::addAction()
//{
//    binder->addItem();
//}

void ProfileTreeWidget::editAction(int itemid, QList<QStringList> *itemlist)
{
    if(itemid < 0) return;
    emit data_editwrite(itemid, itemlist);
}

void ProfileTreeWidget::deleteAction()
{
    binder->deleteItem(fixedCurrentRow());
}

void ProfileTreeWidget::copyAction()
{
    binder->copyItem(fixedCurrentRow());
}

void ProfileTreeWidget::upAction()
{
    binder->upItem(fixedCurrentRow());
}

void ProfileTreeWidget::downAction()
{
    binder->downItem(fixedCurrentRow());
}

void ProfileTreeWidget::reloadAction()
{
    int counter = binder->itemCount();
    qDebug() << "profiletree :: reloadaction";
    this->clear();
    for(int i = 0; i < counter; i++){
        setTreeItem(i);
    }
}

void ProfileTreeWidget::rowSelected()
{
    emit indexChanged(currentRow());
}

void ProfileTreeWidget::popupAction()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/icons/Add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/icons/Denided.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();
//    m_edit = contextMenu->addAction(QIcon(":/icons/Pen.png"), tr("編集"));
//    m_edit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
//    contextMenu->addSeparator();

    m_copy = contextMenu->addAction(QIcon(":/icons/Files_Copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_up = contextMenu->addAction(QIcon(":/icons/Button_Up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/icons/Button_Down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/icons/arrow_refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    //connect signals
    connect(m_add, &QAction::triggered, binder, &FileOperationSignalBinder::addItem);
    connect(m_delete, &QAction::triggered, this, &ProfileTreeWidget::deleteAction);
//    connect(m_edit, &QAction::triggered, this, &ProfileTreeWidget::editAction);
    connect(m_copy, &QAction::triggered, this, &ProfileTreeWidget::copyAction);
    connect(m_up, &QAction::triggered, this, &ProfileTreeWidget::upAction);
    connect(m_down, &QAction::triggered, this, &ProfileTreeWidget::downAction);
    connect(m_ref, &QAction::triggered, this, &ProfileTreeWidget::reloadAction);
}

void ProfileTreeWidget::setTreeItem(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    //reading failure
    if(!binder->readItem(itemid, list)){
        delete list;
        return;
    }

    //get type
    QString type = list->at(0).at(1);

    //set treeitem
    QTreeWidgetItem *root = new QTreeWidgetItem(this);

    //type local
    if(type == "local"){
        delete root;
        return;
    }else{
        root->setText(1, type);
    }

    if(type == "info"){
        setInfoTree(root, list, 1);
    }
    if(type == "normal"){
        setNormalTree(root, list, 1);
    }
    if(type == "search"){
        setSearchTree(root, list, 1);
    }
    if(type == "script"){
        setScriptTree(root, list, 1);
    }
    if(type == "other"){
        setOtherTree(root, list, 1);
    }
    if(type == "temp"){
        setTempTreeItem(root, list);
    }

    qDebug()<< "profiletree::setTreeItem";

    delete list;
}

///DEPENDS_XML
void ProfileTreeWidget::setInfoTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    root->setText(1,"info");
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "(no name)" : curdata;
    root->setText(0, curdata);
    root->setIcon(1, QIcon(":/icons/Info_Light.png"));
}

///DEPENDS_XML
void ProfileTreeWidget::setNormalTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    int cmdskip = VariantConverter::stringToInt(list->at(firstpos + 1).at(1));

    QString curdata;
    QTreeWidgetItem *childitem;
    root->setText(1,"normal");
    root->setIcon(1, QIcon(":/icons/Terminal.png"));
    curdata = (cmdskip == 0)? "NewCommand" : list->at(firstpos + 2).at(1);

    QFileInfo info(curdata);
    if(info.isFile()){
        root->setText(0, info.fileName());
    }else{
        root->setText(0, curdata);
    }

    for(int i = 1; i < cmdskip; i++){
        childitem = new QTreeWidgetItem(root);
        childitem->setText(0, list->at(firstpos + 2 + i).at(1));
        childitem->setText(1, list->at(firstpos + 2 + i).at(3));
    }
}

///DEPENDS_XML
void ProfileTreeWidget::setSearchTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    QTreeWidgetItem *childitem;
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    root->setText(0, curdata);
    root->setText(1,"search");
    root->setIcon(1, QIcon(":/icons/Search.png"));
    childitem = new QTreeWidgetItem(root);
    childitem->setText(0, list->at(firstpos + 1).at(1));
    childitem->setText(0, list->at(firstpos + 2).at(1));
}

///DEPENDS_XML
void ProfileTreeWidget::setScriptTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    QTreeWidgetItem *childitem;

    root->setText(1,"script");
    root->setIcon(1, QIcon(":/icons/File_Text.png"));
    int scrskip = VariantConverter::stringToInt(list->at(firstpos + 2).at(1));
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    QFileInfo info(curdata);
    if(info.isFile()){
        root->setText(0, info.fileName());
    }else{
        root->setText(0, curdata);
    }
    for(int i = 0; i < scrskip; i++){
        childitem = new QTreeWidgetItem(root);
        childitem->setText(0, list->at(firstpos + 3 + i).at(1));
        childitem->setText(1, list->at(firstpos + 3 + i).at(3));
    }
}

///DEPENDS_XML
void ProfileTreeWidget::setOtherTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    root->setText(1,"other");
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    root->setText(0, curdata);
    root->setIcon(1, QIcon(":/icons/Project.png"));
}

int ProfileTreeWidget::currentRow()
{
  QTreeWidgetItem *current = this->currentItem();
  QTreeWidgetItem *first;

  int index = -1;
  int count = this->topLevelItemCount();

  for(int i = 0; i < count; i++){
      first = this->topLevelItem(i);
      if(first == current){
          index = i;
          break;
      }
  }

  return index;
}

int ProfileTreeWidget::rowFromItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *first;

    int index = -1;
    int count = this->topLevelItemCount();

    for(int i = 0; i < count; i++){
        first = this->topLevelItem(i);
        if(first == item){
            index = i;
            break;
        }
    }

    return index;
}

//void ProfileTreeWidget::setSharedFunction(PESharedFunction *func)
//{
//    sfunction = func;
//}

void ProfileTreeWidget::setWidgetsSignalBinder(FileOperationSignalBinder *bind)
{
    binder = bind;

    connect(binder, SIGNAL(refreshFinished()), this, SLOT(reloadAction()));
//    connect(this, SIGNAL(data_editread(int,QList<QStringList>*)), binder, SLOT(editRead(int,QList<QStringList>*)));
    connect(this, SIGNAL(data_editwrite(int,QList<QStringList>*)), binder, SLOT(editWrite(int,QList<QStringList>*)));

    //set right click action
    popupAction();
}

///DEPENDS_XML
void ProfileTreeWidget::setTempTreeItem(QTreeWidgetItem *root, QList<QStringList> *list)
{
    int istack = VariantConverter::stringToInt(list->at(1).at(1));
    int cmdskip = VariantConverter::stringToInt(list->at(3).at(1));

    switch (istack) {
    case 0:
        setNormalTree(root, list, binder->firstPosNormal());
        break;

    case 1:
        setSearchTree(root, list, binder->firstPosSearch());
        break;

    case 2:
        setScriptTree(root, list, binder->firstPosScript() + cmdskip);
        break;

    case 3:
        setOtherTree(root, list, binder->firstPosOther());
        break;

    default:
        break;
    }
}
