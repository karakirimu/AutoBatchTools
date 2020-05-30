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
    QKeyEvent *keyEvent;

    auto cmCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::ControlModifier);
    };

    auto amCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::AltModifier);
    };

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:  if (cmCheck()) addAction();    break;
        case Qt::Key_Delete: if (cmCheck()) deleteAction(); break;

        case Qt::Key_Up:
            if(amCheck()){
                upAction();
            }else{
                if(this->currentRow() != 0)
                    selectRow(this->currentRow() - 1);
            }
            break;

        case Qt::Key_Down:
            if(amCheck()){
                downAction();
            }else{
                if(this->rowCount() - 1 != this->currentRow())
                    selectRow(this->currentRow() + 1);
            }
            break;

        case Qt::Key_X:  if(cmCheck()) cutAction();    break;
        case Qt::Key_C:  if(cmCheck()) copyAction();   break;
        case Qt::Key_V:  if(cmCheck()) pasteAction();  break;
        case Qt::Key_R:  if(cmCheck()) reloadAction(); break;

        default:
            //qDebug("Ate key press %d", keyEvent->key());
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
    int cur = uiIndexToData();
    if(cur > 1){
        editop->cutAction(cur);
        emit editop->processIndexUpdate(cur, -1, EditOperator::DELETE);
    }
}

void ProcessFlowTable::copyAction()
{
    int cur = uiIndexToData();
    if(cur > 1){
        editop->copyAction(cur);
    }
}

void ProcessFlowTable::pasteAction()
{
    int cur = uiIndexToData();
    if(cur > 0){
        cur++;
        editop->pasteAction(cur);
        emit editop->processIndexUpdate(cur, -1, EditOperator::INSERT);

    }
}

void ProcessFlowTable::upAction()
{
    int cur = uiIndexToData();
    if(cur > 2){
        editop->swapAction(cur, cur - 1);
        emit editop->processIndexUpdate(cur - 1, cur, EditOperator::SWAP);
    }
}

void ProcessFlowTable::downAction()
{
    int cur = uiIndexToData();
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

    clearSelection();
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

    int command = sep.last().toInt();

    switch (command) {
    case UiCommandMap::UNDOREDO_EDIT:
    case UiCommandMap::E_EDIT_TABLE:
        replaceItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case CommandMap::UNDOREDO_ADD:
        addItem();
        break;

    case CommandMap::UNDOREDO_DELETE:
        deleteItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case CommandMap::UNDOREDO_INSERT:
        insertItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case CommandMap::UNDOREDO_SWAP:
        swapItem(sep.at(0).toInt(), sep.at(1).toInt());
        break;

    case CommandMap::UNDOREDO_MOVE:
        reloadAction();
        break;

    case UiCommandMap::E_MOVE_TABLE:
    case UiCommandMap::PL_MOVE_TABLE:
        replaceItem(static_cast<QString>(sep.at(0)).toInt());
        break;
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

    replaceItem(before);
    replaceItem(after);

    this->clearSelection();
    this->selectRow(dataToUiIndex(after));

}

void ProcessFlowTable::replaceItem(int id)
{
    qDebug() << "[ProcessFlowTable::replaceItem] rowpos " << id;

//    this->takeItem(dataToUiIndex(id), 0);
    this->removeCellWidget(dataToUiIndex(id), 0);

    setFlowItem(id);
}

//void ProcessFlowTable::moveItem(int before, int beforecount, int after)
//{
//    QHash<int, QString> column0;
//    QHash<int, QTableWidgetItem> column1;

//    if(before < after){
//        for (int i = 0; i < beforecount; i++) {
//            column0.insert(before + i, this->model()->index(before + i, 0).data().toString());
//            column1.insert(before + i, *this->item(before + i, 1));
//        }

//    }else{
//        int bc = before - beforecount + 1;
//        for (int i = 0; i < beforecount; i++) {
//            column0.insert(bc + i, this->model()->index(bc + i, 0).data().toString());
//            column1.insert(bc + i, *this->item(bc + i, 1));
//        }
//    }

//    int deleterow = 0;
//    bool firstelement = false;
//    bool lastelement = false;

//    //    int updown = 0;
//    QString beforedata;
//    QTableWidgetItem beforeval;
//    int deductnum = 0;

//    for (int i = 0; i < beforecount; i++) {

//        if(before > after){

//            if(!lastelement){
//                lastelement = true;
//                deleterow = before;
//            }

//            beforedata = column0.value(before - deductnum);
//            beforeval = column1.value(before - deductnum);
//            deductnum++;

//        }else{

//            if(!firstelement){
//                firstelement = true;
//                deleterow = before;
//            }

//            beforedata = column0.value(before + i);
//            beforeval = column1.value(before + i);
//        }

//        this->blockSignals(true);
//        this->removeRow(deleterow);
//        this->insertRow(after);
//        this->setItem(after, 0, new QTableWidgetItem(beforedata));
//        this->setItem(after, 1, new QTableWidgetItem(beforeval));
//        this->blockSignals(false);
//    }
//}

void ProcessFlowTable::selectChanged(int crow, int ccol, int prow, int pcol)
{
    Q_UNUSED(ccol)
    Q_UNUSED(pcol)
    if(crow == prow) return;

    qDebug() << "[ProcessFlowTable::selectChanged]    rowpos : " << uiIndexToData();
    this->currentItem()->setToolTip(this->currentItem()->text());

    emit editop->processIndexUpdate(uiIndexToData(), -1, EditOperator::SELECT);
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

/**
 * @fn ProcessFlowTable::uiIndexToData
 * @brief Convert index on UI to data number of XML. (To avoid XML localvalue items.)
 * @return fixed id
 */
int ProcessFlowTable::uiIndexToData()
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
    m_up = addTableAction(ACTION::UP, Qt::ALT + Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::ALT + Qt::Key_Down);

    contextMenu->addSeparator();

    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL + Qt::Key_R);

    connect(m_up, &QAction::triggered, this, &ProcessFlowTable::upAction);
    connect(m_down, &QAction::triggered, this, &ProcessFlowTable::downAction);
    connect(m_ref, &QAction::triggered, this, &ProcessFlowTable::reloadAction);
}

void ProcessFlowTable::setFlowItem(int itemid)
{
    EditorCache list;
    FunctionType ft;

    //reading failure
    if(!editop->read(itemid, &list)){
        return;
    }

    switch (ft.getType(list.type)) {
    case ft.TYPE::ALLINCLUDE:  setAllIncludeItem(&list, itemid);  break;
    case ft.TYPE::INFORMATION: setInfoItem(&list, itemid);        break;
    case ft.TYPE::LOCAL:       break;
    case ft.TYPE::EXECUTE:     setExecuteItem(&list, itemid);     break;
    case ft.TYPE::FILESEARCH:  setFileSearchItem(&list, itemid);  break;
    case ft.TYPE::PLUGIN:      setPluginItem(&list, itemid);      break;
    case ft.TYPE::PROFILELOAD: setProfileLoadItem(&list, itemid); break;
    case ft.TYPE::INVALID:     break;
    }

}

void ProcessFlowTable::setAllFlowItem()
{
    QList<EditorCache> list;
    editop->readAll(&list);

    EditorCache inner;
    FunctionType ft;

    int count = list.count();
    for (int n = 0; n < count; n++) {
        inner = list.at(n);

        switch (ft.getType(inner.type)) {
        case ft.TYPE::ALLINCLUDE:  setAllIncludeItem(&inner, n);  break;
        case ft.TYPE::INFORMATION: setInfoItem(&inner, n);        break;
        case ft.TYPE::LOCAL:       break;
        case ft.TYPE::EXECUTE:     setExecuteItem(&inner, n);     break;
        case ft.TYPE::FILESEARCH:  setFileSearchItem(&inner, n);  break;
        case ft.TYPE::PLUGIN:      setPluginItem(&inner, n);      break;
        case ft.TYPE::PROFILELOAD: setProfileLoadItem(&inner, n); break;
        case ft.TYPE::INVALID:     break;
        }
    }
}


void ProcessFlowTable::setAllIncludeItem(EditorCache *list, int dataid)
{
    switch (static_cast<TAB>(list->functionSelect)) {
    case TAB::EXECUTE:
        setExecuteItem(list, dataid);
        break;

    case TAB::FILESEARCH:
        setFileSearchItem(list, dataid);
        break;

    case TAB::PLUGINS:
        setPluginItem(list, dataid);
        break;

    case TAB::PROFILELOAD:
        setProfileLoadItem(list, dataid);
        break;

    case TAB::INVALID:
        break;
    }
}

void ProcessFlowTable::setInfoItem(EditorCache *list, int dataid)
{
    QString curdata = list->info.name;
    curdata = (curdata == "")? "(no name)" : curdata;

    QColor color(tr("#f8f8f8"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(curdata));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(info_pixmap), info_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

void ProcessFlowTable::setExecuteItem(EditorCache *list, int dataid)
{
    int cmdcount = list->exec.command.count();

    QString curdata = (cmdcount == 0)? "(no command)" : list->exec.command.first();
    if(curdata == "") curdata = "(no command)";

    QFileInfo info(curdata);
    QString tmp = info.isFile() ? info.fileName() : curdata;

    for(int i = 1; i < cmdcount; i++){
        tmp.append(" " + list->exec.command.at(i));
    }

    QColor color;
    color.setNamedColor(tr("#e2f6ff"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(tmp));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(exec_pixmap), exec_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

void ProcessFlowTable::setFileSearchItem(EditorCache *list, int dataid)
{
    QString curdata;
    curdata = list->filesearch.name;
    curdata = (curdata == "")? "(not selected) " : (curdata + " ");

    QString curdata2;
    curdata2 = list->filesearch.separator;
    curdata2 = (curdata2 == "")? "(not defined) " : curdata2;

    QString tmp = curdata + QString("sep:%1 ").arg(curdata2);

    //variant or output
    QString cur3;
    if(list->filesearch.outputOption == list->filesearch.VARIANT){

        cur3 = list->filesearch.variant;
        cur3 = (cur3 == "")? "(not defined) " : cur3;

        tmp.append(QString("var:%1").arg(cur3));
    }else{

        cur3 = list->filesearch.filePath;
        cur3 = (cur3 == "")? "(not selected) " : cur3;

        tmp.append(QString("outpath:%1").arg(cur3));
    }

    QColor color(tr("#dcedc8"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(tmp));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(search_pixmap), search_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

void ProcessFlowTable::setPluginItem(EditorCache *list, int dataid)
{
    QString curdata = list->plugin.filePath;

    QFileInfo info(curdata);
    QString tmp = (info.isFile() ? info.baseName() : curdata);

    curdata = (curdata == "")? "(not selected)" : tmp;

    for(QString com : list->plugin.command){
        tmp.append(" " + com);
    }

    QColor color(tr("#ffcdd2"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(curdata));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(plugin_pixmap), plugin_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}

void ProcessFlowTable::setProfileLoadItem(EditorCache *list, int dataid)
{
    QString curdata = list->profileload.filePath;
    QFileInfo profile(curdata);

    if(profile.exists()){
        //read file
        ProcessXmlBuilder tpxb;
        ProcessXmlListGenerator pxlg;
        QList<QStringList> tlist;
        tpxb.setLoadPath(curdata);

        if(tpxb.readItem(0, &tlist)){
            ProcessXmlConstant pxc;
            curdata = pxlg.fetch(pxc.TAG_I_NAME, &tlist);
            curdata.append(" - ");
            curdata.append(profile.baseName());
        }

    }else{
        curdata = tr("(file is not exist)");
    }

    if(curdata == "") curdata = "(not selected)";

    QColor color(tr("#ffecb3"));

    this->setItem(dataToUiIndex(dataid), FIRST, new QTableWidgetItem(curdata));

    QTableWidgetItem *item = new QTableWidgetItem(QIcon(other_pixmap), other_title);
    item->setBackground(QBrush(color));
    item->setForeground(QBrush(QColor(Qt::black)));
    this->setItem(dataToUiIndex(dataid), SECOND, item);
}
