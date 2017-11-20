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

    //setdelegate
//    /*setItemDelegate*/(new FlowDelegate());

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
    emit editop->ui_selectindexUpdate(cache, EditOperator::GRAPHICAREA);
    emit editop->ui_funcindexUpdate(cache, -1, EditOperator::ADD, EditOperator::GRAPHICAREA);

    //testing
//    FlowCellWidget *cell = new FlowCellWidget();
//    int hsize = cell->height();

//    this->setRowHeight(row, hsize);
//    this->setCellWidget(row, 0, cell);
}

void FlowTable::deleteAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->deleteAction(cur);
        deleteItem(cur);
        emit editop->ui_selectindexUpdate(cur, EditOperator::GRAPHICAREA);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::DELETE, EditOperator::GRAPHICAREA);

    }
}

void FlowTable::cutAction()
{
    int cur = fixedCurrentRow();
    if(cur > 1){
        editop->cutAction(cur);
        deleteItem(cur);
        emit editop->ui_selectindexUpdate(cur, EditOperator::GRAPHICAREA);
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
        emit editop->ui_selectindexUpdate(cur, EditOperator::GRAPHICAREA);
        emit editop->ui_funcindexUpdate(cur, -1, EditOperator::INSERT, EditOperator::GRAPHICAREA);

    }
}

void FlowTable::upAction()
{
    int cur = fixedCurrentRow();
    if(cur > 2){
        editop->swapAction(cur, cur - 1);
        swapItem(cur, cur - 1);
        emit editop->ui_selectindexUpdate(cur - 1, EditOperator::GRAPHICAREA);
        emit editop->ui_funcindexUpdate(cur - 1, cur, EditOperator::SWAP, EditOperator::GRAPHICAREA);
    }
}

void FlowTable::downAction()
{
    int cur = fixedCurrentRow();
    if(cur < rowCount()){
        editop->swapAction(cur, cur + 1);
        swapItem(cur, cur + 1);
        emit editop->ui_selectindexUpdate(cur + 1, EditOperator::GRAPHICAREA);
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

    setRowCount(fixedRowFromId(counter));
    for(int i = 0; i < counter; i++){
        setFlowItem(i, false);
    }
    //hide last widget index arrow
    updateLastIndexItem(counter - 1);

//    if(counter > 2){
//        FlowCellWidget *cell = dynamic_cast<FlowCellWidget *>(this->cellWidget(counter - 1, 0));
//        cell->hideArrow();
//    }
}

void FlowTable::addItem()
{
    int row = this->rowCount();
//    this->setRowCount(row + 1);
    insertRow(row);
    setFlowItem(row + 1, false);
    updateLastIndexItem(row + 1);
//    this->setCurrentCell(fixedRowFromId(id),0);
}

void FlowTable::deleteItem(int id)
{
    int uid = fixedRowFromId(id);
    if(uid > 0){
//        delete this->cellWidget(uid, 0);
        this->takeItem(uid, 0);
        this->removeCellWidget(uid, 0);
//        delete this->takeItem(uid, 0);

        this->removeRow(uid);

//        selectChanged(uid - 1, 0, -1, -1);

        //TODO:
        if(rowCount() == uid){
            updateLastIndexItem(uid);
        }

//        this->setCurrentCell(uid-1, 0);
        this->selectRow(uid - 1);
//        int rowc = this->rowCount();
//        if(uid < (rowc - 1)){
//            for(int i = uid + 1; i < rowc; i++){

//            }
//        }
//        this->setItem(uid, 0, new QTableWidgetItem());
//        this->removeRow(uid);
//        this->setRowCount(uid);
    }
}

void FlowTable::insertItem(int id)
{
    int uid = fixedRowFromId(id);
    if(uid > 0){
//        this->setRowCount(this->rowCount() + 1);
        this->insertRow(uid);
        replaceItem(id);

//        this->takeItem(uid + 1, 0);
//        this->removeCellWidget(uid + 1, 0);
//        setFlowItem(id + 1, false);

//        replaceItem(id);
    }
}

void FlowTable::swapItem(int before, int after)
{
//    int uibd = fixedRowFromId(before);
//    int uiad = fixedRowFromId(after);

//    FlowCellWidget *bef = qobject_cast<FlowCellWidget *>(this->cellWidget(uibd, 0));
//    FlowCellWidget *aft = qobject_cast<FlowCellWidget *>(this->cellWidget(uiad, 0));

//    this->takeItem(uibd, 0);
//    this->takeItem(uiad, 0);

//    this->removeCellWidget(before, 0);
//    this->removeCellWidget(after, 0);

//    this->setCellWidget(before, 0, aft);
//    this->setCellWidget(after, 0, bef);

    this->takeItem(fixedRowFromId(before), 0);
    this->removeCellWidget(fixedRowFromId(before), 0);
    setFlowItem(before, false);

    replaceItem(after);

    //arrow TODO:
    int cache = editop->getCacheSize() - 1;
    if(cache == after){
        updateLastIndexItem(after);
    }
    if(cache == before){
        updateLastIndexItem(before);
    }

//    selectRow(after);
//    delete this->cellWidget(before,0);

//    this->takeItem(uibd, 0);
//    this->removeCellWidget(uibd, 0);
//    setFlowItem(uibd, false);

//    delete this->cellWidget(after,0);
//    this->takeItem(uiad, 0);
//    this->removeCellWidget(uiad, 0);
//    setFlowItem(uiad, true);


    //show widget
//    this->selectRow(uibd);
//    this->selectRow(uiad);
//    setCurrentCell(after, 0);
}

void FlowTable::replaceItem(int id)
{
    qDebug() << "EditorTab : replaceFlow";
//    delete this->cellWidget(fixedRowFromId(id),0);
    this->takeItem(fixedRowFromId(id), 0);
    this->removeCellWidget(fixedRowFromId(id), 0);
    setFlowItem(id, true);
    if(fixedRowFromId(editop->getCacheSize()) == id){
        updateLastIndexItem(id);
    }
}

void FlowTable::selectChanged(int crow, int ccol, int prow, int pcol)
{
    qDebug() << "FlowTable::selectChanged";
    FlowCellWidget *cell;

    //unselect prev
    cell = qobject_cast<FlowCellWidget *>(this->cellWidget(prow, pcol));
    if(cell != nullptr) cell->unSelectedItem();

    cell = qobject_cast<FlowCellWidget *>(this->cellWidget(crow, ccol));
    if(cell != nullptr) cell->selectedItem();

    emit editop->ui_selectindexUpdate(fixedCurrentRow(), EditOperator::GRAPHICAREA);
}

void FlowTable::onItemStatusChanged(int after, int before, int function, int sendfrom)
{
    if(sendfrom == EditOperator::GRAPHICAREA) return;

    switch (function) {
    case EditOperator::ADD:
        addItem();
        break;
    case EditOperator::DELETE:
        deleteItem(after);
        break;
    case EditOperator::INSERT:
        insertItem(after);
        break;
    case EditOperator::SWAP:
        swapItem(before, after);
        break;
    default:
        break;
    }
}

int FlowTable::fixedCurrentRow()
{
    int current = this->currentRow();
    return (current > 0)? current + 1 : 0;
}

int FlowTable::fixedRowFromId(int id)
{
    return (id > 1)? id - 1 : 0;
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
    m_add = contextMenu->addAction(QIcon(":/icons/Add.png"),tr("Add"));
    m_add->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter));
    m_delete = contextMenu->addAction(QIcon(":/icons/Denided.png"), tr("Delete"));
    m_delete->setShortcut(QKeySequence(Qt::Key_Delete));
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, SIGNAL(triggered()), this, SLOT(addAction()));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(deleteAction()));
}

void FlowTable::setPopupActionDefault()
{
    m_cut = contextMenu->addAction(tr("Cut"));
    m_cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    m_copy = contextMenu->addAction(QIcon(":/icons/Files_Copy.png"), tr("Copy"));
    m_copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_paste = contextMenu->addAction(QIcon(":/icons/Clipboard_Full.png"), tr("Paste"));
    m_paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    contextMenu->addSeparator();

    connect(m_cut, &QAction::triggered, this, &FlowTable::cutAction);
    connect(m_copy, &QAction::triggered, this, &FlowTable::copyAction);
    connect(m_paste, &QAction::triggered, this, &FlowTable::pasteAction);
}

void FlowTable::setPopupActionBottom()
{
    m_up = contextMenu->addAction(QIcon(":/icons/Button_Up.png"), tr("Up"));
    m_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_down = contextMenu->addAction(QIcon(":/icons/Button_Down.png"), tr("Down"));
    m_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    contextMenu->addSeparator();
    m_ref = contextMenu->addAction(QIcon(":/icons/arrow_refresh.png"), tr("Reload"));
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

    //set treeitem
    FlowCellWidget *cell = new FlowCellWidget();
//    FlowCellData celldata;


    //type local
    if(type == "local"){
//        delete celldata;
        return;
    }

    if(type == "info"){
//        setInfoItem(&celldata, list, 1);
        setInfoItem(cell, list, 1);
    }
    if(type == "normal"){
//        setNormalItem(&celldata, list, 1);
        setNormalItem(cell, list, 1);
    }
    if(type == "search"){
//        setSearchItem(&celldata, list, 1);
        setSearchItem(cell, list, 1);
    }
    if(type == "script"){
//        setExtraFuncItem(&celldata, list, 1);
        setExtraFuncItem(cell, list, 1);
    }
    if(type == "other"){
//        setOtherItem(&celldata, list, 1);
        setOtherItem(cell, list, 1);
    }
    if(type == "temp"){
//        setTempItem(&celldata, list);
        setTempItem(cell, list);
    }

    //set new item
//    QTableWidgetItem *flowitem = new QTableWidgetItem();
//    flowitem->setData(0, QVariant::fromValue(celldata));
//    setItem(fixedRowFromId(itemid), 0, flowitem);

    this->setRowHeight(fixedRowFromId(itemid), cell->height());
    if(selector) cell->selectedItem();
    this->setCellWidget(fixedRowFromId(itemid), 0, cell);
    //cell->show();

    qDebug()<< "profileflow::setFlowItem";

    delete list;
}

void FlowTable::setTempItem(FlowCellWidget *cell, QList<QStringList> *list)
{
    int istack = VariantConverter::stringToInt(list->at(1).at(1));

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
        setExtraFuncItem(cell, list, hlist.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
        break;

    case ProcessXmlListGenerator::OTHER:
        setOtherItem(cell, list, hlist.value(ProcessXmlListGenerator::OTHER) + 1);
        break;

    default:
        break;
    }
}

void FlowTable::setInfoItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "(no name)" : curdata;
//    cell->setType("info", QPixmap(":/icons/Info_Light.png"));
    cell->setType("Information");
    cell->setTypepixmap(QIcon(":/icons/Info_Light.png").pixmap(16,16));
    cell->setTypecolor("color: black; background-color: rgb(230, 230, 230);");
    cell->setFramecolor("background-color: rgb(120, 120, 120);");
    cell->hideArrow();
    //TODO:content

    QString tmp = getHtmlHeader("");
    tmp.append(QString("project name : %1<br>").arg(list->at(1).at(1)));
    tmp.append(QString("version      : %1<br>").arg(list->at(2).at(1)));
    tmp.append(QString("author       : %1<br>").arg(list->at(3).at(1)));
    tmp.append(QString("description  :<br>%1<br>").arg(list->at(4).at(1)));
    tmp.append(QString("last updated : %1").arg(list->at(5).at(1)));
    tmp.append(getHtmlFooter());

    cell->setContent(tmp);
//    node->addLines(QStringList() << "info" << "-----" << curdata);
//    node->setPath(QColor(120,120,120), 2, QColor(230,230,230));
}

void FlowTable::setNormalItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    int cmdskip = VariantConverter::stringToInt(list->at(firstpos + 1).at(1));

    QString curdata;
//    cell->setType("normal", QPixmap(":/icons/Terminal.png"));
    cell->setType("Executable");
    cell->setTypepixmap(QIcon(":/icons/Terminal.png").pixmap(16,16));
    cell->setTypecolor("color: black; background-color: rgb(222, 235, 247);");
    cell->setFramecolor("background-color: rgb(44, 70, 94);");
    curdata = (cmdskip == 0)? "NewCommand" : list->at(firstpos + 2).at(1);

    QString tmp = getHtmlHeader("");

    QFileInfo info(curdata);
    if(info.isFile()){
        tmp.append(info.fileName());
    }else{
        tmp.append(curdata);
    }
    tmp.append("<br>");

    for(int i = 1; i < cmdskip; i++){
        tmp.append(list->at(firstpos + 2 + i).at(1));
        if(i < cmdskip - 1){
            tmp.append("<br>");
        }
    }

    tmp.append(getHtmlFooter());

    cell->setContent(tmp);
}

void FlowTable::setSearchItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
//    cell->setType("search", QPixmap(":/icons/Search.png"));
    cell->setType("File Search");
    cell->setTypepixmap(QIcon(":/icons/Search.png").pixmap(16,16));
    cell->setTypecolor("color: black; background-color: rgb(226, 240, 217);");
    cell->setFramecolor("background-color: rgb(56, 87, 35);");

    QString tmp = curdata;
    tmp.append("<br>");
    tmp.append(tr("Separator: %1").arg(list->at(firstpos + 1).at(1)));
    tmp.append("<br>");

    //variant or output
    if(((QString)list->at(firstpos + 3).at(3)).toInt() == 0){
        tmp.append(tr("Variant: %1").arg(list->at(firstpos + 2).at(1)));
    }else{
        tmp.append(tr("Filepath: %1").arg(list->at(firstpos + 3).at(1)));
    }

    cell->setContent(tmp);
}

void FlowTable::setExtraFuncItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata;
//    cell->setType("script", QPixmap(":/icons/File_Text.png"));
    cell->setType("External Function");
    cell->setTypepixmap(QIcon(":/icons/File_Text.png").pixmap(16,16));
    cell->setTypecolor("color: black; background-color: rgb(251, 215, 214);");
    cell->setFramecolor("background-color: rgb(132, 12, 12);");

    QString tmp = "";

    int scrskip = VariantConverter::stringToInt(list->at(firstpos + 2).at(1));
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    QFileInfo info(curdata);
    if(info.isFile()){
        tmp.append(info.fileName());
    }else{
        tmp.append(curdata);
    }
    tmp.append("<br>");

    for(int i = 0; i < scrskip; i++){
        tmp.append(list->at(firstpos + 3 + i).at(1));
        if(i < scrskip - 1){
            tmp.append("<br>");
        }
    }

    cell->setContent(tmp);
}

void FlowTable::setOtherItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos)
{
    QString curdata;
//    cell->setType("other", QPixmap(":/icons/Project.png"));
    cell->setType("Run Other Profile");
    cell->setTypepixmap(QIcon(":/icons/Project.png").pixmap(16,16));
    cell->setTypecolor("color: black; background-color: rgb(255, 242, 204);");
    cell->setFramecolor("background-color: rgb(132, 60, 12);");

    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    cell->setContent(curdata);
}

void FlowTable::updateLastIndexItem(int lastindex)
{
    FlowCellWidget *cell = qobject_cast<FlowCellWidget *>(this->cellWidget(fixedRowFromId(lastindex), 0));
    if(cell != nullptr) cell->hideArrow();

    if((lastindex - 1) > 1){
        cell = qobject_cast<FlowCellWidget *>(this->cellWidget(fixedRowFromId(lastindex - 1), 0));
        if(cell != nullptr) cell->showArrow();
    }
}

QString FlowTable::getHtmlHeader(QString headcssstr)
{
    QString result = QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                             "<html><head><style type=\"text/css\">%1</style></head><body>").arg(headcssstr);
    return result;
}

QString FlowTable::getHtmlFooter()
{
    return "</body></html>";
}
