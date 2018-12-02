#include "processflowtable.h"

ProcessFlowTable::ProcessFlowTable(QWidget *parent)
    : BasicTable(parent)
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

    setHorizontalHeaderLabels((QStringList() << tr("Resource") << tr("Type")));

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set selection changed
    connect(this, &ProcessFlowTable::currentCellChanged, this, &ProcessFlowTable::selectChanged);

}

ProcessFlowTable::~ProcessFlowTable()
{

}

void ProcessFlowTable::setEditOperator(EditOperator *op)
{
    editop = op;

    connect(editop, &EditOperator::editUpdate, this, &ProcessFlowTable::replaceItem);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &ProcessFlowTable::onItemStatusChanged);
}

bool ProcessFlowTable::eventFilter(QObject *obj, QEvent *event)
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

void ProcessFlowTable::addAction()
{
    editop->addAction();
    addItem();

    int cache = editop->getCacheSize() - 1;
    emit editop->ui_funcindexUpdate(cache, -1, EditOperator::ADD, EditOperator::FLOWTABLE);
}

void ProcessFlowTable::deleteAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->deleteAction(cur);
        deleteItem(cur);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::FLOWTABLE);
    }
}

void ProcessFlowTable::cutAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->cutAction(cur);
        deleteItem(cur);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::FLOWTABLE);
    }
}

void ProcessFlowTable::copyAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->copyAction(cur);
    }
}

void ProcessFlowTable::pasteAction()
{
    int cur = fixedCurrentRow();
    if(cur > 0){
        cur++;
        editop->pasteAction(cur);
        insertItem(cur);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::INSERT, EditOperator::FLOWTABLE);

    }
}

void ProcessFlowTable::upAction()
{
    int cur = fixedCurrentRow();
    if(cur > 2){
        editop->swapAction(cur, cur - 1);
        swapItem(cur, cur - 1);
        emit editop->ui_funcindexUpdate(cur - 1, cur, EditOperator::SWAP, EditOperator::FLOWTABLE);
    }
}

void ProcessFlowTable::downAction()
{
    int cur = fixedCurrentRow();
    if(cur < rowCount()){
        editop->swapAction(cur, cur + 1);
        swapItem(cur, cur + 1);
        emit editop->ui_funcindexUpdate(cur + 1, cur, EditOperator::SWAP, EditOperator::FLOWTABLE);
    }
}

void ProcessFlowTable::reloadAction()
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
//    updateLastIndexItem(counter - 1);
}

void ProcessFlowTable::updateIndex(QString operation)
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

        bool ok = false;
        int second = static_cast<QString>(sep.at(1)).toInt(&ok);

        if(ok) swapItem(uiIndexToData(first), uiIndexToData(second));
    }
}

void ProcessFlowTable::addItem()
{
    int row = this->rowCount();
    insertRow(row);
    setFlowItem(uiIndexToData(row));

//    updateLastIndexItem(uiIndexToData(row));
//    excludeSelector(row, 0, this->currentRow(), 0);

}

void ProcessFlowTable::deleteItem(int id)
{
    int uid = dataToUiIndex(id);
    if(uid > 0){
        this->takeItem(uid, 0);
        this->removeCellWidget(uid, 0);
        this->removeRow(uid);

        //TODO:
//        if(rowCount() == uid){
//            updateLastIndexItem(uid);
//        }

        this->selectRow(uid - 1);

//        There may be no previous element
//        excludeSelector(uid - 1, 0, uid - 1, 0);
    }
}

void ProcessFlowTable::insertItem(int id)
{
    int uid = dataToUiIndex(id);
    if(uid > 0){
        this->insertRow(uid);
        replaceItem(id);
        this->selectRow(uid);
    }
}

void ProcessFlowTable::swapItem(int before, int after)
{
    this->takeItem(dataToUiIndex(before), 0);
    this->removeCellWidget(dataToUiIndex(before), 0);
    setFlowItem(before);

    replaceItem(after);

    //arrow TODO:
//    int cache = editop->getCacheSize() - 1;
//    if(cache == after)  updateLastIndexItem(after);
//    if(cache == before) updateLastIndexItem(before);
}

void ProcessFlowTable::replaceItem(int id)
{
    qDebug() << "EditorTab : replaceItem : " << id;
    this->takeItem(dataToUiIndex(id), 0);
    this->removeCellWidget(dataToUiIndex(id), 0);
    setFlowItem(id);

//    if(dataToUiIndex(editop->getCacheSize()) == id){
//        updateLastIndexItem(id);
//    }

//    excludeSelector(dataToUiIndex(id), 0, this->currentRow(), 0);
}

void ProcessFlowTable::selectChanged(int crow, int ccol, int prow, int pcol)
{
    Q_UNUSED(crow); Q_UNUSED(ccol); Q_UNUSED(prow); Q_UNUSED(pcol);

    qDebug() << "FlowTable::selectChanged";
    emit editop->ui_funcindexUpdate(fixedCurrentRow(), -1, EditOperator::SELECT, EditOperator::FLOWTABLE);
}

void ProcessFlowTable::onItemStatusChanged(int after, int before, int function, int sendfrom)
{
#ifdef QT_DEBUG
    qDebug() << "FlowTable::onItemStatusChanged : index : " << after \
             << " function : " << function;

#endif
    if(sendfrom == EditOperator::FLOWTABLE) return;

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

//void ProcessFlowTable::excludeSelector(int crow, int ccol, int prow, int pcol)
//{
//    FlowCellWidget *cell;

//    //cell color change
//    //unselect prev
//    cell = qobject_cast<FlowCellWidget *>(this->cellWidget(prow, pcol));
//    if(cell != nullptr) cell->unSelectedItem();

//    cell = qobject_cast<FlowCellWidget *>(this->cellWidget(crow, ccol));
//    if(cell != nullptr) cell->selectedItem();

//    //table select change
//    this->selectRow(crow);
//}

int ProcessFlowTable::fixedCurrentRow()
{
    return uiIndexToData(this->currentRow());
}

int ProcessFlowTable::dataToUiIndex(int id)
{
    return (id > 1)? id - 1 : 0;
}

int ProcessFlowTable::uiIndexToData(int id)
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

void ProcessFlowTable::setPopupActionTop()
{
    //set basic items
    m_add = contextMenu->addAction(QIcon(":/default_icons/add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/default_icons/remove.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &ProcessFlowTable::addAction);
    connect(m_delete, &QAction::triggered, this, &ProcessFlowTable::deleteAction);
}

void ProcessFlowTable::setPopupActionDefault()
{
    m_cut = contextMenu->addAction(tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/default_icons/copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/default_icons/paste.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    contextMenu->addSeparator();

    connect(m_cut, &QAction::triggered, this, &ProcessFlowTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &ProcessFlowTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &ProcessFlowTable::pasteAction);
}

void ProcessFlowTable::setPopupActionBottom()
{
    m_up = contextMenu->addAction(QIcon(":/default_icons/arrow_up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/default_icons/arrow_down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/default_icons/refresh.png"), tr("Reload"));
    m_ref->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    connect(m_up, &QAction::triggered, this, &ProcessFlowTable::upAction);
    connect(m_down, &QAction::triggered, this, &ProcessFlowTable::downAction);
    connect(m_ref, &QAction::triggered, this, &ProcessFlowTable::reloadAction);
}

void ProcessFlowTable::setFlowItem(int itemid)
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
//    FlowCellWidget *cell = new FlowCellWidget();

    if(type == TYPE_ALLINCLUDE){
        setTempItem(list, itemid);

    }else if(type == TYPE_INFO){
        setInfoItem(list, itemid);

    }else if(type == TYPE_EXEC){
        setNormalItem(list, itemid);

    }else if(type == TYPE_SEARCH){
        setSearchItem(list, itemid);

    }else if(type == TYPE_SCRIPT){
        setPluginsItem(list, itemid);

    }else if(type == TYPE_ANOTHER){
        setOtherItem(list, itemid);

    }

//    this->setRowHeight(dataToUiIndex(itemid), cell->height());
//    if(selector) cell->selectedItem();
//    this->setCellWidget(dataToUiIndex(itemid), 0, cell);
    //cell->show();

    delete list;
}

void ProcessFlowTable::setAllFlowItem()
{
    QList<QList<QStringList> *> *list = new QList<QList<QStringList> *>();
    editop->readAll(list);

    QMutableListIterator<QList<QStringList> *> i(*list);
    QList<QStringList> *inner;
    QString type = "";
//    FlowCellWidget *cell;
    int n = 0;
    while(i.hasNext()){
        inner = i.next();

        //get type
        type = pxlg.fetch(ALL_TYPE, ATTR_NONE, inner);

        //set item
//        cell = new FlowCellWidget();

        if(type == TYPE_ALLINCLUDE){
            setTempItem(inner, n);

        }else if(type == TYPE_INFO){
            setInfoItem(inner, n);

        }else if(type == TYPE_EXEC){
            setNormalItem(inner, n);

        }else if(type == TYPE_SEARCH){
            setSearchItem(inner, n);

        }else if(type == TYPE_SCRIPT){
            setPluginsItem(inner, n);

        }else if(type == TYPE_ANOTHER){
            setOtherItem(inner, n);
        }

//        if(type != TYPE_LOCAL){
//            this->setRowHeight(dataToUiIndex(n), cell->height());
//            this->setCellWidget(dataToUiIndex(n), 0, cell);

//        }else{
//            delete cell;
//        }

        n++;
    }

    delete list;
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setTempItem(QList<QStringList> *list, int dataid)
{
    int istack = static_cast<QString>(pxlg.fetch(TE_STACKEDWIDGET_POSITION, ATTR_NONE, list)).toInt();

    QHash<int, int> hlist;
    pxlg.getListStructure(list, &hlist);

    switch (istack) {
    case ProcessXmlListGenerator::NORMAL:
        setNormalItem(list, dataid);
        break;

    case ProcessXmlListGenerator::SEARCH:
        setSearchItem(list, dataid);
        break;

    case ProcessXmlListGenerator::EXTRAFUNC:
        setPluginsItem(list, dataid);
        break;

    case ProcessXmlListGenerator::OTHER:
        setOtherItem(list, dataid);
        break;

    default:
        break;
    }
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setInfoItem(QList<QStringList> *list, int dataid)
{
    QString curdata = pxlg.fetch(I_NAME, ATTR_NONE, list);
    curdata = (curdata == "")? "(no name)" : curdata;

//    cell->setTypeAll(info_title, &info_pixmap, &info_style, &info_frame);
//    cell->hideArrow();

    QString tmp/* = getHtmlHeader("")*/;
    tmp.append(QString("%1").arg(curdata));
//    tmp.append(QString("[version]: %1　").arg(pxlg.fetch(I_VERSION, ATTR_NONE, list)));
//    tmp.append(QString("[author]: %1　").arg(pxlg.fetch(I_AUTHOR, ATTR_NONE, list)));
//    tmp.append(QString("[description]: %1\n").arg(pxlg.fetch(I_DESCRIPTION, ATTR_NONE, list)));

    QColor color;
    color.setNamedColor(tr("#e8e8e8"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(tmp));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(info_pixmap), info_title);
    item->setBackground(QBrush(color));
    item->setTextColor(QColor(Qt::black));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setNormalItem(QList<QStringList> *list, int dataid)
{
    int cmdcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, list)).toInt();
    int cmdfirst = pxlg.fetchCmdFirstPos(E_CMD, list);

//    cell->setTypeAll(exec_title, &exec_pixmap, &exec_style, &exec_frame);

    QString curdata = (cmdcount == 0)? "NewCommand" : list->at(cmdfirst).at(1);

    QFileInfo info(curdata);
    QString tmp = /*getHtmlHeader("") + */(info.isFile() ? info.fileName() : curdata);

    for(int i = 1; i < cmdcount; i++){
        tmp.append(" " + list->at(cmdfirst + i).at(1));
    }

    QColor color;
    color.setNamedColor(tr("#e2f6ff"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(tmp));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(exec_pixmap), exec_title);
    item->setBackground(QBrush(color));
    item->setTextColor(QColor(Qt::black));
    this->setItem(dataToUiIndex(dataid), SECOND, item);

//    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setSearchItem(QList<QStringList> *list, int dataid)
{
    QString curdata;
    curdata = pxlg.fetch(S_NAME, ATTR_NONE, list);
    curdata = (curdata == "")? "(not selected) " : curdata;

//    cell->setTypeAll(search_title, &search_pixmap, &search_style, &search_frame);

    QString curdata2;
    curdata2 = pxlg.fetch(S_SEPARATOR, ATTR_NONE, list);
    curdata2 = (curdata2 == "")? "(not defined) " : curdata2;

    QString tmp = curdata + QString("sep:%1 ").arg(curdata2);

    //variant or output
    QString cur3;
    if(static_cast<QString>(pxlg.fetch(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, list)).toInt() == 0){

        cur3 = pxlg.fetch(S_VARIANT, ATTR_NONE, list);
        cur3 = (cur3 == "")? "(not defined) " : cur3;

        tmp.append(QString("var:%1").arg(cur3));
    }else{

        cur3 = pxlg.fetch(S_OUTPUTFILE, ATTR_NONE, list);
        cur3 = (cur3 == "")? "(not selected) " : cur3;

        tmp.append(QString("outpath:%1").arg(cur3));
    }

    QColor color;
    color.setNamedColor(tr("#dcedc8"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(tmp));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(search_pixmap), search_title);
    item->setBackground(QBrush(color));
    item->setTextColor(QColor(Qt::black));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
//    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setPluginsItem(QList<QStringList> *list, int dataid)
{
    QString curdata = pxlg.fetch(PL_NAME, ATTR_NONE, list);
    curdata = (curdata == "")? "(not selected)" : curdata;

//    cell->setTypeAll(extra_title, &extra_pixmap, &extra_style, &extra_frame);

    QFileInfo info(curdata);
    QString tmp = (info.isFile() ? info.fileName() : curdata);

    int plgcount = static_cast<QString>(pxlg.fetch(PL_CMDARGCOUNT, ATTR_NONE, list)).toInt();
    int plgfirst = pxlg.fetchCmdFirstPos(PL_CMD, list);

    for(int i = 0; i < plgcount; i++){
        tmp.append(" " + list->at(plgfirst + i).at(1));
    }

    QColor color;
    color.setNamedColor(tr("#ffcdd2"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(curdata));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(plugin_pixmap), plugin_title);
    item->setBackground(QBrush(color));
    item->setTextColor(QColor(Qt::black));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
//    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setOtherItem(QList<QStringList> *list, int dataid)
{
    QString curdata = pxlg.fetch(PR_NAME, ATTR_NONE, list);
    curdata = (curdata == "")? "(not selected)" : curdata;

    QColor color;
    color.setNamedColor(tr("#ffecb3"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(curdata));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(other_pixmap), other_title);
    item->setBackground(QBrush(color));
    item->setTextColor(QColor(Qt::black));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

//void ProcessFlowTable::updateLastIndexItem(int lastindex)
//{
//    FlowCellWidget *cell = qobject_cast<FlowCellWidget *>(this->cellWidget(dataToUiIndex(lastindex), 0));
//    if(cell != nullptr) cell->hideArrow();

//    if((lastindex - 1) > 1){
//        cell = qobject_cast<FlowCellWidget *>(this->cellWidget(dataToUiIndex(lastindex - 1), 0));
//        if(cell != nullptr) cell->showArrow();
//    }
//}

