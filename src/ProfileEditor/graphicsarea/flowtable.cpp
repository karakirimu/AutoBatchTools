#include "flowtable.h"

FlowTable::FlowTable(QWidget *parent)
    : BasicTable(parent)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //init table size
    setColumnCount(1);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //hide header label
    horizontalHeader()->hide();
    verticalHeader()->hide();

    //set selection changed
    connect(this, &FlowTable::currentCellChanged, this, &FlowTable::selectChanged);

}

FlowTable::~FlowTable()
{

}

void FlowTable::setEditOperator(EditOperator *op)
{
    editop = op;

    connect(editop, &EditOperator::editUpdate, this, &FlowTable::replaceItem);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &FlowTable::onItemStatusChanged);
}

bool FlowTable::eventFilter(QObject *obj, QEvent *event)
{
//    qDebug() << "FlowTable::eventFilter : " << event->type();
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:
             if (keyEvent->modifiers() & Qt::ControlModifier)
               addAction();
             //todo
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

void FlowTable::addAction()
{
    editop->addAction();
    addItem();

    int cache = editop->getCacheSize() - 1;
    emit editop->ui_funcindexUpdate(cache, -1, EditOperator::ADD, EditOperator::GRAPHICAREA);
}

void FlowTable::deleteAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->deleteAction(cur);
        deleteItem(cur);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::GRAPHICAREA);
    }
}

void FlowTable::cutAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->cutAction(cur);
        deleteItem(cur);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::GRAPHICAREA);
    }
}

void FlowTable::copyAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->copyAction(cur);
    }
}

void FlowTable::pasteAction()
{
    int cur = fixedCurrentRow();
    if(cur > 0){
        cur++;
        editop->pasteAction(cur);
        insertItem(cur);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::INSERT, EditOperator::GRAPHICAREA);

    }
}

void FlowTable::upAction()
{
    int cur = fixedCurrentRow();
    if(cur > 2){
        editop->swapAction(cur, cur - 1);
        swapItem(cur, cur - 1);
        emit editop->ui_funcindexUpdate(cur - 1, cur, EditOperator::SWAP, EditOperator::GRAPHICAREA);
    }
}

void FlowTable::downAction()
{
    int cur = fixedCurrentRow();
    if(cur < rowCount()){
        editop->swapAction(cur, cur + 1);
        swapItem(cur, cur + 1);
        emit editop->ui_funcindexUpdate(cur + 1, cur, EditOperator::SWAP, EditOperator::GRAPHICAREA);
    }
}

void FlowTable::reloadAction()
{
    int counter = editop->getCacheSize();
    qDebug() << "flowtable :: reloadaction";
    //can't clear, delete all objects
    while(this->rowCount() > 0){
        this->cellWidget(0, 0);
        this->removeCellWidget(0,0);
        this->removeRow(0);
    }

    setRowCount(dataToUiIndex(counter));

#ifdef QT_DEBUG
    qDebug()<< "profileflow::setFlowItem::reloadaction";
    QTime time;
    time.start();
#endif

    setAllFlowItem();

#ifdef QT_DEBUG
    qDebug() << "FlowTable::reloadAction() || elapsed: " << time.elapsed() << "ms";
#endif

    //hide last widget index arrow
    updateLastIndexItem(counter - 1);
}

void FlowTable::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");

    if(sep.count() < 2){
        //edit
        replaceItem(static_cast<QString>(sep.at(0)).toInt());
    }else if(sep.at(1) == UNDOREDO_ADD){
        //add
        addItem();
    }else if(sep.at(1) == UNDOREDO_DELETE){
        //del
        deleteItem(static_cast<QString>(sep.at(0)).toInt());
    }else if(sep.at(1) == UNDOREDO_INSERT){
        //ins
        insertItem(static_cast<QString>(sep.at(0)).toInt());
    }else{
        //swap
        int first = static_cast<QString>(sep.at(0)).toInt();
        int second = static_cast<QString>(sep.at(1)).toInt();
        swapItem(uiIndexToData(first), uiIndexToData(second));
    }
}

void FlowTable::addItem()
{
    int row = this->rowCount();
    insertRow(row);
    setFlowItem(uiIndexToData(row), false);

    updateLastIndexItem(uiIndexToData(row));
    excludeSelector(row, 0, this->currentRow(), 0);

}

void FlowTable::deleteItem(int id)
{
    int uid = dataToUiIndex(id);
    if(uid > 0){
        this->takeItem(uid, 0);
        this->removeCellWidget(uid, 0);
        this->removeRow(uid);

        //TODO:
        if(rowCount() == uid){
            updateLastIndexItem(uid);
        }

        this->selectRow(uid - 1);

//        There may be no previous element
        excludeSelector(uid - 1, 0, uid - 1, 0);
    }
}

void FlowTable::insertItem(int id)
{
    int uid = dataToUiIndex(id);
    if(uid > 0){
        this->insertRow(uid);
        replaceItem(id);
    }
}

void FlowTable::swapItem(int before, int after)
{
    this->takeItem(dataToUiIndex(before), 0);
    this->removeCellWidget(dataToUiIndex(before), 0);
    setFlowItem(before, false);

    replaceItem(after);

    //arrow TODO:
    int cache = editop->getCacheSize() - 1;
    if(cache == after)  updateLastIndexItem(after);
    if(cache == before) updateLastIndexItem(before);
}

void FlowTable::replaceItem(int id)
{
    qDebug() << "EditorTab : replaceItem : " << id;
    this->takeItem(dataToUiIndex(id), 0);
    this->removeCellWidget(dataToUiIndex(id), 0);
    setFlowItem(id, false);

    if(dataToUiIndex(editop->getCacheSize()) == id){
        updateLastIndexItem(id);
    }

    excludeSelector(dataToUiIndex(id), 0, this->currentRow(), 0);
}

void FlowTable::selectChanged(int crow, int ccol, int prow, int pcol)
{
    qDebug() << "FlowTable::selectChanged";
    excludeSelector(crow, ccol, prow, pcol);

    emit editop->ui_funcindexUpdate(fixedCurrentRow(), -1, EditOperator::SELECT, EditOperator::GRAPHICAREA);
}

void FlowTable::onItemStatusChanged(int after, int before, int function, int sendfrom)
{
#ifdef QT_DEBUG
    qDebug() << "FlowTable::onItemStatusChanged : index : " << after \
             << " function : " << function;

#endif
    if(sendfrom == EditOperator::GRAPHICAREA) return;

    switch (function) {
    case EditOperator::ADD:
    case EditOperator::INSERT:
        insertItem(after);
        break;
    case EditOperator::DELETE:
        deleteItem(after);
        break;
    case EditOperator::SWAP:
        swapItem(before, after);
        break;
    case EditOperator::SELECT:
        this->selectRow(dataToUiIndex(after));
        break;
    default:
        break;
    }
}

void FlowTable::excludeSelector(int crow, int ccol, int prow, int pcol)
{
    FlowCellWidget *cell;

    //cell color change
    //unselect prev
    cell = qobject_cast<FlowCellWidget *>(this->cellWidget(prow, pcol));
    if(cell != nullptr) cell->unSelectedItem();

    cell = qobject_cast<FlowCellWidget *>(this->cellWidget(crow, ccol));
    if(cell != nullptr) cell->selectedItem();

    //table select change
    this->selectRow(crow);
}

int FlowTable::fixedCurrentRow()
{
    return uiIndexToData(this->currentRow());
}

int FlowTable::dataToUiIndex(int id)
{
    return (id > 1)? id - 1 : 0;
}

int FlowTable::uiIndexToData(int id)
{
    return (id > 0)? id + 1 : 0;
}

//void FlowTable::mousePressEvent(QMouseEvent *event)
//{
//    QModelIndexList list = this->selectedIndexes();

//    if(!list.isEmpty()){
//        int count = list.count();
//        for(int i = 0; i < count; i++){
//            FlowCellWidget *prev = dynamic_cast<FlowCellWidget *>(this->indexWidget(list.at(i)));
//            prev->unSelectedItem();
//        }
//    }

//    if(event->button() == Qt::LeftButton){
//        QWidget *widget = this->indexWidget(this->indexAt(event->pos()));
//        FlowCellWidget *cell = dynamic_cast<FlowCellWidget *>(widget);
//        cell->selectedItem();
//    }
//}

void FlowTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &FlowTable::addAction);
    connect(m_delete, &QAction::triggered, this, &FlowTable::deleteAction);
}

void FlowTable::setPopupActionDefault()
{
    m_cut = contextMenu->addAction(tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/default_icons/copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/default_icons/paste.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    contextMenu->addSeparator();

    connect(m_cut, &QAction::triggered, this, &FlowTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &FlowTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &FlowTable::pasteAction);
}

void FlowTable::setPopupActionBottom()
{
    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_up, &QAction::triggered, this, &FlowTable::upAction);
    connect(m_down, &QAction::triggered, this, &FlowTable::downAction);
    connect(m_ref, &QAction::triggered, this, &FlowTable::reloadAction);
}

void FlowTable::setFlowItem(int itemid, bool selector)
{
    QList<QStringList> *list = new QList<QStringList>();

    //reading failure
    if(!editop->read(itemid, list)){
        delete list;
        return;
    }

    //get type
    QString type = list->at(0).at(1);

    //type local
    if(type == TYPE_LOCAL) return;

    //set treeitem
    FlowCellWidget *cell = new FlowCellWidget();

    if(type == TYPE_ALLINCLUDE){
        setTempItem(cell, list);

    }else if(type == TYPE_INFO){
        setInfoItem(cell, list, 1);

    }else if(type == TYPE_EXEC){
        setNormalItem(cell, list, 1);

    }else if(type == TYPE_SEARCH){
        setSearchItem(cell, list, 1);

    }else if(type == TYPE_SCRIPT){
        setPluginsItem(cell, list, 1);

    }else if(type == TYPE_ANOTHER){
        setOtherItem(cell, list, 1);

    }

    this->setRowHeight(dataToUiIndex(itemid), cell->height());
    if(selector) cell->selectedItem();
    this->setCellWidget(dataToUiIndex(itemid), 0, cell);
    //cell->show();

    delete list;
}

void FlowTable::setAllFlowItem()
{
    QList<QList<QStringList> *> *list = new QList<QList<QStringList> *>();
    editop->readAll(list);

    QMutableListIterator<QList<QStringList> *> i(*list);
    QList<QStringList> *inner;
    QString type = "";
    FlowCellWidget *cell;
    int n = 0;
    while(i.hasNext()){
        inner = i.next();

        //get type
        type = inner->at(0).at(1);

        //set item
        cell = new FlowCellWidget();

        if(type == TYPE_ALLINCLUDE){
            setTempItem(cell, inner);

        }else if(type == TYPE_INFO){
            setInfoItem(cell, inner, 1);

        }else if(type == TYPE_EXEC){
            setNormalItem(cell, inner, 1);

        }else if(type == TYPE_SEARCH){
            setSearchItem(cell, inner, 1);

        }else if(type == TYPE_SCRIPT){
            setPluginsItem(cell, inner, 1);

        }else if(type == TYPE_ANOTHER){
            setOtherItem(cell, inner, 1);
        }

        if(type != TYPE_LOCAL){
            this->setRowHeight(dataToUiIndex(n), cell->height());
            this->setCellWidget(dataToUiIndex(n), 0, cell);

        }else{
            delete cell;
        }

        n++;
    }

    delete list;
}

///DEPENDS_XML DEPENDS_UI PROCESS
void FlowTable::setTempItem(FlowCellWidget *cell, QList<QStringList> *list)
{
    int istack = static_cast<QString>(list->at(1).at(1)).toInt();

    QHash<int, int> hlist;
    xgen.getListStructure(list, &hlist);

    switch (istack) {
    case ProcessXmlListGenerator::NORMAL:
        setNormalItem(cell, list, hlist.value(ProcessXmlListGenerator::NORMAL) + 1);
        break;

    case ProcessXmlListGenerator::SEARCH:
        setSearchItem(cell, list, hlist.value(ProcessXmlListGenerator::SEARCH) + 1);
        break;

    case ProcessXmlListGenerator::EXTRAFUNC:
        setPluginsItem(cell, list, hlist.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
        break;

    case ProcessXmlListGenerator::OTHER:
        setOtherItem(cell, list, hlist.value(ProcessXmlListGenerator::OTHER) + 1);
        break;

    default:
        break;
    }
}

///DEPENDS_XML DEPENDS_UI PROCESS
void FlowTable::setInfoItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "(no name)" : curdata;

    cell->setTypeAll(info_title, &info_pixmap, &info_style, &info_frame);
    cell->hideArrow();

    QString tmp/* = getHtmlHeader("")*/;
    tmp.append(QString("[project]     : %1<br>").arg(list->at(1).at(1)));
    tmp.append(QString("[version]     : %1<br>").arg(list->at(2).at(1)));
    tmp.append(QString("[author]      : %1<br>").arg(list->at(3).at(1)));
    tmp.append(QString("[description] : %1<br>").arg(list->at(4).at(1)));

    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void FlowTable::setNormalItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    int cmdskip = QString(list->at(firstpos + 1).at(1)).toInt();

    cell->setTypeAll(exec_title, &exec_pixmap, &exec_style, &exec_frame);

    QString curdata = (cmdskip == 0)? "NewCommand" : list->at(firstpos + 2).at(1);

    QFileInfo info(curdata);
    QString tmp = /*getHtmlHeader("") + */(info.isFile() ? info.fileName() : curdata);

    for(int i = 1; i < cmdskip; i++){
        tmp.append("<br>" + list->at(firstpos + 2 + i).at(1));
    }

    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void FlowTable::setSearchItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;

    cell->setTypeAll(search_title, &search_pixmap, &search_style, &search_frame);

    QString tmp = curdata + "<br>" \
            + tr("[separator] : %1").arg(list->at(firstpos + 1).at(1)) + "<br>";

    //variant or output
    if(static_cast<QString>(list->at(firstpos + 3).at(3)).toInt() == 0){
        tmp.append(tr("[variant]   : %1").arg(list->at(firstpos + 2).at(1)));
    }else{
        tmp.append(tr("[filepath]  : %1").arg(list->at(firstpos + 3).at(1)));
    }

    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void FlowTable::setPluginsItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;

    cell->setTypeAll(extra_title, &extra_pixmap, &extra_style, &extra_frame);

    QFileInfo info(curdata);
    QString tmp = (info.isFile() ? info.fileName() : curdata);

    int scrskip = QString(list->at(firstpos + 2).at(1)).toInt();
    for(int i = 0; i < scrskip; i++){
        tmp.append("<br>" + list->at(firstpos + 3 + i).at(1));
    }

    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void FlowTable::setOtherItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;

    cell->setTypeAll(other_title, &other_pixmap, &other_style, &other_frame);
    cell->setContent(curdata);
}

void FlowTable::updateLastIndexItem(int lastindex)
{
    FlowCellWidget *cell = qobject_cast<FlowCellWidget *>(this->cellWidget(dataToUiIndex(lastindex), 0));
    if(cell != nullptr) cell->hideArrow();

    if((lastindex - 1) > 1){
        cell = qobject_cast<FlowCellWidget *>(this->cellWidget(dataToUiIndex(lastindex - 1), 0));
        if(cell != nullptr) cell->showArrow();
    }
}

