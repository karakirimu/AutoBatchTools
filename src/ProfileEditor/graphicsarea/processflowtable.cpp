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

#include "processflowtable.h"

ProcessFlowTable::ProcessFlowTable(QWidget *parent)
    : BasicTable(parent)
{
    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
//    setDragDropMode(QAbstractItemView::InternalMove);

    //init table size
    setColumnCount(2);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    setHorizontalHeaderLabels((QStringList() << tr("Resource") << tr("Type")));

    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //set header style
    verticalHeader()->setProperty("VerticalHeaderStyle", 1);

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
    connect(editop, &EditOperator::processIndexUpdate, this, &ProcessFlowTable::onItemStatusChanged);
}

bool ProcessFlowTable::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        switch (keyEvent->key())
         {
           case Qt::Key_Up:
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             break;

           case Qt::Key_Down:
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
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
    return BaseTable::eventFilter(obj, event);
}

void ProcessFlowTable::addAction()
{
    editop->addAction();

    int cache = editop->getCacheSize() - 1;
    emit editop->processIndexUpdate(cache, -1, EditOperator::ADD);

}


/**
 * @fn ProcessFlowTable::deleteAction
 * @brief Clear multiple selections at once.
 */
void ProcessFlowTable::deleteAction()
{
    // information process only
    if(this->rowCount() < 2) return;

    //check delete warning message
    if(!deleteCheckMessage()) return;

    int cur = 0;

    QModelIndexList lists = this->selectionModel()->selectedRows();

    std::sort(lists.rbegin(), lists.rend());


    for (int i = 0; i < lists.count(); i++) {
        cur = uiIndexToData(lists.at(i).row());

        if(cur == 0) break;

        editop->deleteAction(cur);
        emit editop->processIndexUpdate(cur, -1, EditOperator::DELETE);
    }
}

void ProcessFlowTable::cutAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->cutAction(cur);
        emit editop->processIndexUpdate(cur, -1, EditOperator::DELETE);
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
        emit editop->processIndexUpdate(cur, -1, EditOperator::INSERT);

    }
}

void ProcessFlowTable::upAction()
{
    int cur = fixedCurrentRow();
    if(cur > 2){
        editop->swapAction(cur, cur - 1);
        emit editop->processIndexUpdate(cur - 1, cur, EditOperator::SWAP);
    }
}

void ProcessFlowTable::downAction()
{
    int cur = fixedCurrentRow();
    if(cur < rowCount()){
        editop->swapAction(cur, cur + 1);
        emit editop->processIndexUpdate(cur + 1, cur, EditOperator::SWAP);
    }
}

/**
 * @fn ProcessFlowTable::reloadAction
 * @brief Reload process flow items.
 */
void ProcessFlowTable::reloadAction()
{
    // Reflect after update
    int cur = this->currentRow();

    this->blockSignals(true);

    int counter = editop->getCacheSize();

    //can't clear, delete all objects
    while(this->rowCount() > 0){
        this->removeRow(0);
    }

    setRowCount(dataToUiIndex(counter));

#ifdef QT_DEBUG
    qDebug()<< "[ProcessFlowTable::reloadAction] Timer start";
    QTime time;
    time.start();
#endif

    setAllFlowItem();

#ifdef QT_DEBUG
    qDebug() << "[ProcessFlowTable::reloadAction] elapsed : " << time.elapsed() << "ms";
#endif
    this->blockSignals(false);

    selectRow(cur);
}

/**
 * @fn ProcessFlowTable::updateIndex
 * @brief Function called when back / forward is executed
 * @param operation Formatted string
 */
void ProcessFlowTable::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");

    if(sep.count() < 2) return;

    if(sep.at(1) == UNDOREDO_EDIT || \
            (sep.count() == 3 && sep.at(2) == UNDOREDO_E_TABLEEDIT)){
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
    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_SWAP){
        //swap
        int first = static_cast<QString>(sep.at(0)).toInt();
        int second = static_cast<QString>(sep.at(1)).toInt();

        swapItem(first, second);
    }else if(sep.count() == 4
             && sep.at(3) == UNDOREDO_MOVE){
        //move
        reloadAction();

    }else if(sep.count() == 5
             && (sep.at(4) == UNDOREDO_E_TABLEMOVE
                 || sep.at(4) == UNDOREDO_PL_TABLEMOVE)){
        replaceItem(static_cast<QString>(sep.at(0)).toInt());
    }
}

void ProcessFlowTable::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "[ProcessFlowTable::dragEnterEvent] Object : " << event->source()->objectName();
    if(event->source() != nullptr
            && this->indexAt(event->pos()).row() > 0 ){
        event->acceptProposedAction();
    }
}

void ProcessFlowTable::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "[ProcessFlowTable::dragMoveEvent] Object : " << event->source()->objectName();
    if(event->source() != nullptr
            && this->indexAt(event->pos()).row() > 0 ){
        event->accept();
    }else{
        event->ignore();
    }
}

void ProcessFlowTable::dropEvent(QDropEvent *event)
{
    QList<int> beforeindex;

    int droppedrow = this->indexAt(event->pos()).row();
    if(droppedrow == 0) return;

    if(!BaseTable::insideDropRowsMove(event, &beforeindex)) return;

    for (int i = 0; i < beforeindex.count(); i++) {
        beforeindex[i] = uiIndexToData(beforeindex.at(i));
    }

    qDebug() << "[ProcessFlowTable::dropEvent] droppedrow : " << droppedrow;

    editop->dragDropAction(beforeindex, uiIndexToData(droppedrow));

}

void ProcessFlowTable::addItem()
{
    int row = this->rowCount();

    insertRow(row);
    setFlowItem(uiIndexToData(row));

    this->selectRow(row);
}

void ProcessFlowTable::deleteItem(int id)
{
    int uid = dataToUiIndex(id);
    if(uid < 1) return;

    this->takeItem(uid, 0);
    this->removeCellWidget(uid, 0);
    this->removeRow(uid);

    this->selectRow(uid - 1);
}

void ProcessFlowTable::insertItem(int id)
{
    int uid = dataToUiIndex(id);
    if(uid < 1) return;

    this->clearSelection();

    this->insertRow(uid);
    replaceItem(id);
    this->selectRow(uid);
}

void ProcessFlowTable::swapItem(int before, int after)
{
    this->clearSelection();

    replaceItem(before);
    replaceItem(after);

    this->selectRow(dataToUiIndex(after));

}

void ProcessFlowTable::replaceItem(int id)
{
    qDebug() << "[ProcessFlowTable::replaceItem] rowpos " << id;

    this->takeItem(dataToUiIndex(id), 0);
    this->removeCellWidget(dataToUiIndex(id), 0);

    setFlowItem(id);
}

void ProcessFlowTable::selectChanged(int crow, int ccol, int prow, int pcol)
{
    Q_UNUSED(ccol)
    Q_UNUSED(pcol)
    if(crow == prow) return;

    qDebug() << "[ProcessFlowTable::selectChanged]    rowpos : " << fixedCurrentRow();
    this->currentItem()->setToolTip(this->currentItem()->text());

    emit editop->processIndexUpdate(fixedCurrentRow(), -1, EditOperator::SELECT);
}

void ProcessFlowTable::onItemStatusChanged(int after, int before, int function)
{


#ifdef QT_DEBUG
    qDebug() << "[ProcessFlowTable::onItemStatusChanged] rowpos : after : " << after \
             << " before : " << before \
             << " function : " << function;

#endif

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
        break;
    default:
        break;
    }
}

//! \fn ProcessFlowTable::fixedCurrentRow
//! \brief Convert index on UI to data number of XML. (To avoid XML localvalue items.)
//! \return fixed id
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

//void ProcessFlowTable::mousePressEvent(QMouseEvent *event)
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
    m_add = addTableAction(ACTION::ADD, Qt::CTRL + Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::CTRL + Qt::Key_Delete);

    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &ProcessFlowTable::addAction);
    connect(m_delete, &QAction::triggered, this, &ProcessFlowTable::deleteAction);
}

void ProcessFlowTable::setPopupActionDefault()
{
    m_cut = addTableAction(ACTION::CUT, Qt::CTRL + Qt::Key_X);
    m_copy = addTableAction(ACTION::COPY, Qt::CTRL + Qt::Key_C);
    m_paste = addTableAction(ACTION::PASTE, Qt::CTRL + Qt::Key_V);

    contextMenu->addSeparator();

    connect(m_cut, &QAction::triggered, this, &ProcessFlowTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &ProcessFlowTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &ProcessFlowTable::pasteAction);
}

void ProcessFlowTable::setPopupActionBottom()
{
    m_up = addTableAction(ACTION::UP, Qt::CTRL + Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL + Qt::Key_Down);

    contextMenu->addSeparator();

    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL + Qt::Key_R);

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
    QString type = pxlg.fetch(ALL_TYPE, ATTR_NONE, list);

    //type local
    if(type == TYPE_LOCAL) return;

    if(type == TYPE_ALLINCLUDE){    setTempItem(list, itemid);

    }else if(type == TYPE_INFO){    setInfoItem(list, itemid);

    }else if(type == TYPE_EXEC){    setNormalItem(list, itemid);

    }else if(type == TYPE_SEARCH){  setSearchItem(list, itemid);

    }else if(type == TYPE_SCRIPT){  setPluginsItem(list, itemid);

    }else if(type == TYPE_ANOTHER){ setProfileItem(list, itemid);

    }

    delete list;
}

void ProcessFlowTable::setAllFlowItem()
{
    QList<QList<QStringList> *> *list = new QList<QList<QStringList> *>();
    editop->readAll(list);

    QMutableListIterator<QList<QStringList> *> i(*list);
    QList<QStringList> *inner;
    QString type = "";

    int n = 0;
    while(i.hasNext()){

        inner = i.next();

        //get type
        type = pxlg.fetch(ALL_TYPE, ATTR_NONE, inner);

        if(type == TYPE_ALLINCLUDE){    setTempItem(inner, n);

        }else if(type == TYPE_INFO){    setInfoItem(inner, n);

        }else if(type == TYPE_EXEC){    setNormalItem(inner, n);

        }else if(type == TYPE_SEARCH){  setSearchItem(inner, n);

        }else if(type == TYPE_SCRIPT){  setPluginsItem(inner, n);

        }else if(type == TYPE_ANOTHER){ setProfileItem(inner, n);

        }

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

    case ProcessXmlListGenerator::PLUGINS:
        setPluginsItem(list, dataid);
        break;

    case ProcessXmlListGenerator::OTHER:
        setProfileItem(list, dataid);
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
    color.setNamedColor(tr("#f8f8f8"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(tmp));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(info_pixmap), info_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setNormalItem(QList<QStringList> *list, int dataid)
{
    int cmdcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, list)).toInt();
    int cmdfirst = pxlg.fetchCmdFirstPos(E_CMD, list);

//    cell->setTypeAll(exec_title, &exec_pixmap, &exec_style, &exec_frame);

    QString curdata = (cmdcount == 0)? "(no command)" : list->at(cmdfirst).at(1);
    if(curdata == "") curdata = "(no command)";

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
    item->setForeground(QBrush(QColor(Qt::black)));
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
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
//    cell->setContent(tmp);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void ProcessFlowTable::setPluginsItem(QList<QStringList> *list, int dataid)
{
    QString curdata = pxlg.fetch(PL_FILEPATH, ATTR_NONE, list);

//    cell->setTypeAll(extra_title, &extra_pixmap, &extra_style, &extra_frame);

    QFileInfo info(curdata);
    QString tmp = (info.isFile() ? info.baseName() : curdata);

    curdata = (curdata == "")? "(not selected)" : tmp;

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
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
//    cell->setContent(tmp);
}

/**
 * @fn ProcessFlowTable::setProfileItem
 * @brief DEPENDS_XML DEPENDS_UI
 * @param list loaded xml list
 * @param dataid xml data position
 */
void ProcessFlowTable::setProfileItem(QList<QStringList> *list, int dataid)
{
    QString curdata = pxlg.fetch(PR_FILEPATH, ATTR_NONE, list);
    QFileInfo profile(curdata);

    if(profile.exists()){
        //read file
        ProcessXmlBuilder tpxb;
        QList<QStringList> tlist;
        tpxb.setLoadPath(curdata);

        if(tpxb.readItem(0, &tlist)){
            curdata = pxlg.fetch(I_NAME, ATTR_NONE, &tlist);
            curdata.append(" - ");
            curdata.append(profile.baseName());
        }

    }else{
        curdata = tr("(file is not exist)");
    }

    if(curdata == "") curdata = "(not selected)";

    QColor color;
    color.setNamedColor(tr("#ffecb3"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(curdata));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(other_pixmap), other_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}
