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

int ProfileTreeWidget::fixedRowFromId(int id)
{
    return (id > 1)? id - 1 : 0;
}

void ProfileTreeWidget::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

void ProfileTreeWidget::onItemStatusChanged(int after, int before, int function, int sendfrom)
{
    if(sendfrom == EditOperator::TREE) return;

    switch (function) {
    case EditOperator::ADD:
        replaceTree(after);
        break;
    case EditOperator::DELETE:
        deleteTree(after);
        break;
    case EditOperator::INSERT:
        insertTree(after);
        break;
    case EditOperator::SWAP:
        swapTree(fixedRowFromId(before), fixedRowFromId(after));
        break;
    default:
        break;
    }
}

bool ProfileTreeWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
       QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
       switch (keyEvent->key()){
           case Qt::Key_Return:
           case Qt::Key_Enter:
             if (keyEvent->modifiers() & Qt::ControlModifier){
    //                binder->addItem();
                 addAction();
             }else{
                 if(this->selectedItems().count() > 0){
                     this->setItemSelected(this->selectedItems().at(0), false);
                 }
                 this->setItemSelected(this->currentItem(), true);
             }
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
                 if(this->currentRow() != this->topLevelItemCount() - 1)
                     this->setCurrentItem(this->topLevelItem(this->currentRow() + 1));
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

    //           case Qt::Key_E:
    //             if (keyEvent->modifiers() & Qt::ControlModifier)
    //                //emit editActionKeyPressed();
    //             break;

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

//void ProfileTreeWidget::editAction(int itemid, QList<QStringList> *itemlist)
//{
//    if(itemid < 0) return;
//    emit data_editwrite(itemid, itemlist);
//}


void ProfileTreeWidget::addAction()
{
    editop->addAction();
    int count = editop->getCacheSize() - 1;
    count = (count >= 0) ? count : 0;
    addTree(count);
    emit editop->ui_selectindexUpdate(count, EditOperator::TREE);
    emit editop->ui_funcindexUpdate(count, -1, EditOperator::ADD, EditOperator::TREE);

}

void ProfileTreeWidget::deleteAction()
{
//    binder->deleteItem(fixedCurrentRow());
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->deleteAction(cur);
        deleteTree(cur);
        emit editop->ui_selectindexUpdate(cur, EditOperator::TREE);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::TREE);

    }
}

void ProfileTreeWidget::cutAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->cutAction(cur);
        deleteTree(cur);
        emit editop->ui_selectindexUpdate(cur, EditOperator::TREE);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::TREE);
    }
}

void ProfileTreeWidget::copyAction()
{
//    binder->copyItem(fixedCurrentRow());
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->copyAction(cur);
    }
}

void ProfileTreeWidget::pasteAction()
{
    int cur = fixedCurrentRow();
//    int uicur = currentRow();
    if(cur > 0){
        cur++;
        editop->pasteAction(cur);
        insertTree(cur);
        emit editop->ui_selectindexUpdate(cur, EditOperator::TREE);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::INSERT, EditOperator::TREE);

    }
}

void ProfileTreeWidget::upAction()
{
//    binder->upItem(fixedCurrentRow());
    int cur = fixedCurrentRow();
    int uicur = currentRow();
    if(uicur > 1){
        editop->swapAction(cur, cur - 1);
        swapTree(uicur, uicur - 1);
        emit editop->ui_selectindexUpdate(cur - 1, EditOperator::TREE);
        emit editop->ui_funcindexUpdate(cur - 1, cur, EditOperator::SWAP, EditOperator::TREE);

    }
}

void ProfileTreeWidget::downAction()
{
//    binder->downItem(fixedCurrentRow());
    int cur = fixedCurrentRow();
    int uicur = currentRow();
    if(uicur < topLevelItemCount() - 1){
        editop->swapAction(cur, cur + 1);
        swapTree(uicur, uicur + 1);
        emit editop->ui_selectindexUpdate(cur + 1, EditOperator::TREE);
        emit editop->ui_funcindexUpdate(cur + 1, cur, EditOperator::SWAP, EditOperator::TREE);

    }
}

void ProfileTreeWidget::reloadAction()
{
    int counter = editop->getCacheSize();
    qDebug() << "profiletree :: reloadaction";
    this->clear();
    for(int i = 0; i < counter; i++){
        setTree(i);
    }
}

void ProfileTreeWidget::addTree(int id)
{
     setTree(id);
     this->setCurrentItem(this->topLevelItem(fixedRowFromId(id)));
}

void ProfileTreeWidget::deleteTree(int id)
{
    int fid = fixedRowFromId(id);
    if(fid > 0){
        this->takeTopLevelItem(fid);
    }
}

void ProfileTreeWidget::insertTree(int id)
{
    setTree(id);
    int laindex = this->topLevelItemCount() - 1;
    QTreeWidgetItem *item = this->takeTopLevelItem(laindex);
    this->insertTopLevelItem(fixedRowFromId(id), item);

    //reselect
//    emit editop->ui_selectindexUpdate(id - 1, EditOperator::TREE);
}

void ProfileTreeWidget::swapTree(int before, int after)
{
    QTreeWidgetItem *item_small;
    QTreeWidgetItem *item_big;

//    QTreeWidgetItem *item_before;
//    QTreeWidgetItem *item_after;

    //take bigger index first
    if(before < after){
        //down
        item_big = this->topLevelItem(after);
        item_small = this->topLevelItem(before);

        this->takeTopLevelItem(after);
        this->insertTopLevelItem(after, item_small);

//        this->takeTopLevelItem(before);
        this->insertTopLevelItem(before, item_big);

    }else{
        //up
        item_big = this->topLevelItem(before);
        item_small = this->topLevelItem(after);

        this->takeTopLevelItem(after);
        this->insertTopLevelItem(after, item_big);
        this->insertTopLevelItem(before, item_small);
    }

    if(this->selectedItems().count() > 0){
        this->setItemSelected(this->selectedItems().at(0), false);
    }
    this->setItemSelected(this->topLevelItem(after), true);

//    QTreeWidgetItem *item_before = this->topLevelItem(before);
//    QTreeWidgetItem *item_after = this->topLevelItem(after);

//    QTreeWidgetItem *item_before = this->topLevelItem(before);
//    this->insertTopLevelItem(before, item_after);
//    this->insertTopLevelItem(after, item_before);
}

void ProfileTreeWidget::replaceTree(int id)
{
    qDebug() << "EditorTab : replaceTree";
    //todo:
    //settree xml based id, but this treewidget depends on fixed id.
    setTree(id);
    int fid = fixedRowFromId(id);
    int laindex = this->topLevelItemCount() - 1;
    QTreeWidgetItem *item = this->takeTopLevelItem(laindex);
    delete this->takeTopLevelItem(fid);
    this->insertTopLevelItem(fid, item);
    this->setCurrentItem(this->topLevelItem(fid));
}

void ProfileTreeWidget::rowSelected()
{
    emit editop->ui_selectindexUpdate(fixedCurrentRow(), EditOperator::TREE);
//    emit indexChanged(currentRow());
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

    m_cut = contextMenu->addAction(tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/icons/Files_Copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/icons/Clipboard_Full.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    contextMenu->addSeparator();

    m_up = contextMenu->addAction(QIcon(":/icons/Button_Up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/icons/Button_Down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/icons/arrow_refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    //connect signals
    connect(m_add, &QAction::triggered, this, &ProfileTreeWidget::addAction);
    connect(m_delete, &QAction::triggered, this, &ProfileTreeWidget::deleteAction);
//    connect(m_edit, &QAction::triggered, this, &ProfileTreeWidget::editAction);
    connect(m_cut, &QAction::triggered, this, &ProfileTreeWidget::cutAction);
    connect(m_copy, &QAction::triggered, this, &ProfileTreeWidget::copyAction);
    connect(m_paste, &QAction::triggered, this, &ProfileTreeWidget::pasteAction);

    connect(m_up, &QAction::triggered, this, &ProfileTreeWidget::upAction);
    connect(m_down, &QAction::triggered, this, &ProfileTreeWidget::downAction);
    connect(m_ref, &QAction::triggered, this, &ProfileTreeWidget::reloadAction);
}

void ProfileTreeWidget::setTree(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    //reading failure
    if(!editop->read(itemid, list)){
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
        setExtraFuncTree(root, list, 1);
    }
    if(type == "other"){
        setOtherTree(root, list, 1);
    }
    if(type == "temp"){
        setTempTree(root, list);
    }

    qDebug()<< "profiletree::setTreeItem";

    delete list;
}

///DEPENDS_XML
void ProfileTreeWidget::setInfoTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    root->setText(1,"Info");
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
    root->setText(1,"Exec");
    root->setIcon(1, QIcon(":/icons/Terminal.png"));
    curdata = (cmdskip == 0)? "NewCommand" : list->at(firstpos + 2).at(1);

    QFileInfo info(curdata);
    if(info.isFile()){
        root->setText(0, info.fileName());
    }else{
        root->setText(0, curdata);
    }

    QString tmp = "";

    for(int i = 1; i < cmdskip; i++){
        childitem = new QTreeWidgetItem(root);
        childitem->setText(0, list->at(firstpos + 2 + i).at(1));

        tmp = list->at(firstpos + 2 + i).at(3);

        if(tmp == "0"){
            tmp = tr("program");
        }else{
            tmp = tr("arg %1").arg(tmp);
        }

        childitem->setText(1, tmp);
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
    root->setText(1,"Search");
    root->setIcon(1, QIcon(":/icons/Search.png"));
    childitem = new QTreeWidgetItem(root);
    childitem->setText(0, list->at(firstpos + 1).at(1));
    childitem->setText(1, tr("Separator"));

    //variant or output
    childitem = new QTreeWidgetItem(root);

    if(((QString)list->at(firstpos + 3).at(3)).toInt() == 0){
        childitem->setText(0, list->at(firstpos + 2).at(1));
        childitem->setText(1, tr("Variant"));
    }else{
        childitem->setText(0, list->at(firstpos + 3).at(1));
        childitem->setText(1, tr("Filepath"));
    }
}

///DEPENDS_XML
void ProfileTreeWidget::setExtraFuncTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    QTreeWidgetItem *childitem;

    root->setText(1,"External");
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

    QString tmp;

    for(int i = 0; i < scrskip; i++){
        childitem = new QTreeWidgetItem(root);
        childitem->setText(0, list->at(firstpos + 3 + i).at(1));

        tmp = tr("arg %1")
                .arg(QString::number(((QString)list->at(firstpos + 3 + i).at(3)).toInt() + 1));
        childitem->setText(1, tmp);
    }
}

///DEPENDS_XML
void ProfileTreeWidget::setOtherTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    root->setText(1,"Profile");
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

//void ProfileTreeWidget::setWidgetsSignalBinder(FileOperationSignalBinder *bind)
//{
//    binder = bind;

//    connect(binder, SIGNAL(refreshFinished()), this, SLOT(reloadAction()));
////    connect(this, SIGNAL(data_editread(int,QList<QStringList>*)), binder, SLOT(editRead(int,QList<QStringList>*)));
//    connect(this, SIGNAL(data_editwrite(int,QList<QStringList>*)), binder, SLOT(editWrite(int,QList<QStringList>*)));

//    //set right click action
//    popupAction();
//}

void ProfileTreeWidget::setEditOperator(EditOperator *op)
{
    editop = op;

    connect(editop, &EditOperator::editUpdate, this, &ProfileTreeWidget::replaceTree);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &ProfileTreeWidget::onItemStatusChanged);

    //set right click action
    popupAction();
}

///DEPENDS_XML
void ProfileTreeWidget::setTempTree(QTreeWidgetItem *root, QList<QStringList> *list)
{
    int istack = VariantConverter::stringToInt(list->at(1).at(1));
//    int cmdskip = VariantConverter::stringToInt(list->at(3).at(1));

    QHash<int, int> hlist;
    xgen.getListStructure(list, &hlist);

    switch (istack) {
    case ProcessXmlListGenerator::NORMAL:
        setNormalTree(root, list, hlist.value(ProcessXmlListGenerator::NORMAL) + 1);
        break;

    case ProcessXmlListGenerator::SEARCH:
        setSearchTree(root, list, hlist.value(ProcessXmlListGenerator::SEARCH) + 1);
        break;

    case ProcessXmlListGenerator::EXTRAFUNC:
        setExtraFuncTree(root, list, hlist.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
        break;

    case ProcessXmlListGenerator::OTHER:
        setOtherTree(root, list, hlist.value(ProcessXmlListGenerator::OTHER) + 1);
        break;

    default:
        break;
    }
}
