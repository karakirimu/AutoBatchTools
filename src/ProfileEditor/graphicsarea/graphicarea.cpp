#include "graphicarea.h"

GraphicArea::GraphicArea(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    QRectF current = this->rect();
    qreal w = (current.width() > MINWIDTH ? current.width() : MINWIDTH);
    scene->setSceneRect(-w, -10,500, 0);
    setScene(scene);
    setCacheMode(CacheNone);
    setViewportUpdateMode(SmartViewportUpdate);
    setRenderHint(QPainter::NonCosmeticDefaultPen);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    //install key event filter
    installEventFilter(this);

    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenu(QPoint)));

    //init menu context
    contextMenu = new QMenu(this);

    //init nodearrowlist
    nodelist = new NodeArrowList(this);

    //init selectedindex
    selectedIndex = 0;

//    nodelist->setScene(scene);

    //nodetest
//    centerNode = new BaseNode(this);
//    centerNode->addLines(QStringList() << "a" << "b" << "c");
//    centerNode->setPath(QColor(44,70,94), 2, QColor(222,235,247));
//    scene->addItem(centerNode);
//    centerNode->setPos(0,0);
//    BaseNode *node1 = new BaseNode(this);
//    node1->addLines(QStringList() << "This is a test program"
//                    << "0099ew09e9ewuw9r"
//                    << "ffodskapso@:;:;"
//                    << "longtextsssssssssssssssssssssssssssssssssssssss");
//    node1->setPath(QColor(120,120,120), 2, QColor(230,230,230));
//    scene->addItem(node1);

//    scene->addItem(new Arrow(node1, centerNode));

//    BaseNode *node2 = new BaseNode(this);
//    node2->addLines(QStringList() << "This is a test program"
//                    << "0099ewewerewrewr09e9ewuw9r"
//                    << "f3omfep0"
//                    << "wehoiewihrewoi");
//    node2->setPath(QColor(56,87,35), 2, QColor(226,240,217));
//    scene->addItem(node2);
//    scene->addItem(new Arrow(centerNode, node2));


//    BaseNode *node3 = new BaseNode(this);
//    node3->addLines(QStringList() << "This is a test program"
//                    << "0099eww9r"
//                    << "f3omfep0"
//                    << "wehoiewihrewoi");
//    node3->setPath(QColor(132,12,12), 2, QColor(251,215,214));
//    scene->addItem(node3);
//    scene->addItem(new Arrow(node2, node3));

//    BaseNode *node4 = new BaseNode(this);
//    node4->addLines(QStringList() << "This is a test program"
//                    << "009htybtr"
//                    << "f3nhynny0"
//                    << "wrehretrewoi");
//    node4->setPath(QColor(132,60,12), 2, QColor(255,242,204));
//    scene->addItem(node4);
//    scene->addItem(new Arrow(node3, node4));
    //background initial


}

GraphicArea::~GraphicArea()
{
    delete nodelist;
}

void GraphicArea::itemMoved()
{
    //timer loop in 40 msec
    if (!timerId)
        timerId = startTimer(40);
}

void GraphicArea::itemSelectChanged(int index)
{
    selectedIndex = index;
//    emit selectChangedAction(index);
    emit editop->selectindexUpdate(index, EditOperator::GRAPHICAREA);
}

//void GraphicArea::setWidgetsSignalBinder(FileOperationSignalBinder *bind)
//{

//    binder = bind;
//    connect(binder, SIGNAL(refreshFinished()), this, SLOT(reloadAction()));
////    connect(this, SIGNAL(data_add()), binder, SLOT(addItem()));
////    connect(this, SIGNAL(data_insert(int)), binder, SLOT(insertItem(int)));
////    connect(this, SIGNAL(data_delete(int)), binder, SLOT(deleteItem(int)));
////    connect(this, SIGNAL(data_replace(int,bool)), binder, SLOT(replace(int,bool)));
////    connect(this, SIGNAL(data_editread(int,QList<QStringList>*)), binder, SLOT(editRead(int,QList<QStringList>*)));
//    //    connect(this, SIGNAL(data_editwrite(int,QList<QStringList>*)), binder, SLOT(editWrite(int,QList<QStringList>*)));
//}

void GraphicArea::setEditOperator(EditOperator *op)
{
    editop = op;

    connect(editop, &EditOperator::editUpdate, this, &GraphicArea::replaceItem);
    //set popup action
    popupAction();
}

void GraphicArea::zoomIn()
{
//    scaleView(qreal(1.2));
}

void GraphicArea::zoomOut()
{
    //    scaleView(1 / qreal(1.2));
}

void GraphicArea::addAction()
{
//    emit data_add();
//    binder->addItem();
    editop->addAction();
    int count = editop->getCacheSize() - 1;
    count = (count >= 0) ? count : 0;
    addItem(count);

    emit editop->selectindexUpdate(count, EditOperator::GRAPHICAREA);
}

//void GraphicArea::editAction(int itemid, QList<QStringList> *itemlist)
//{
//    if(itemid < 0) return;
//    emit data_editwrite(itemid, itemlist);
//}

void GraphicArea::deleteAction()
{
//    emit data_delete(0);
//    binder->deleteItem(this->scene()->selectedItems() );
//    binder->deleteItem(selectedIndex);
    editop->deleteAction(selectedIndex);
    emit editop->selectindexUpdate(selectedIndex, EditOperator::GRAPHICAREA);
}

void GraphicArea::cutAction()
{
    editop->cutAction(selectedIndex);
    emit editop->selectindexUpdate(selectedIndex, EditOperator::GRAPHICAREA);
}

void GraphicArea::copyAction()
{
//    binder->copyItem(selectedIndex);
    editop->copyAction(selectedIndex);
}

void GraphicArea::pasteAction()
{
    editop->pasteAction(selectedIndex);
    emit editop->selectindexUpdate(selectedIndex, EditOperator::GRAPHICAREA);
}

void GraphicArea::upAction()
{
//    binder->upItem(selectedIndex);
    editop->swapAction(selectedIndex, selectedIndex - 1);
    emit editop->selectindexUpdate(selectedIndex, EditOperator::GRAPHICAREA);
}

void GraphicArea::downAction()
{
//    binder->downItem(selectedIndex);
    editop->swapAction(selectedIndex, selectedIndex + 1);
    emit editop->selectindexUpdate(selectedIndex, EditOperator::GRAPHICAREA);
}

void GraphicArea::reloadAction()
{
    int counter = editop->getCacheSize();
    qDebug() << "graphicarea :: reloadaction";
    //TODO dynamic addition
    scene()->items().clear();
    nodelist->clearlist();

    for(int i = 0; i < counter; i++){
        setItem(i);
    }

//    counter = nodelist->nodecount();



//    for(int i = 0; i < counter; i++){

//    }

}

void GraphicArea::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    //update scenerect
    QRectF current = this->rect();
    qreal w = (current.width() > MINWIDTH ? current.width() : MINWIDTH);
    scene()->setSceneRect(-w, -10, w, 150 * editop->getCacheSize());

    //update nodepoint
    QList<BaseNode *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (BaseNode *node = qgraphicsitem_cast<BaseNode *>(item))
            nodes << node;
    }

    foreach (BaseNode *node, nodes)
        node->itemMoving();

    bool itemsMoved = false;
    foreach (BaseNode *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphicArea::drawBackground(QPainter *painter, const QRectF &rect)
{
//    QVector<QLineF> lines;
//    //prepare linepoints
//    qreal tl_x = rect.topLeft().rx();
//    qreal tr_x = rect.topRight().rx();
//    qreal tl_y = rect.topLeft().ry();
//    qreal bl_y = rect.bottomLeft().ry();

//    for(qreal i = tl_x; i < tr_x; i+=20){
//        lines.append(QLineF(i, tl_y, i, bl_y));
//    }

//    for(qreal i = tl_y; i < bl_y; i+=20){
//        lines.append(QLineF(tl_x, i, tr_x, i));
//    }

    painter->fillRect(rect, Qt::gray);
    //grid line
    painter->fillRect(rect, QBrush(Qt::lightGray, Qt::CrossPattern));
//    painter->fillRect(rect, Qt::lightGray);
//    painter->drawLines(lines);

}

//#ifndef QT_NO_WHEELEVENT
//void GraphicArea::wheelEvent(QWheelEvent *event)
//{
//    Q_UNUSED(event);
//    //    scaleView(pow((double)2, -event->delta() / 240.0));

//}
//#endif

void GraphicArea::resizeEvent(QResizeEvent *event)
{
    //TODO: scenerect
    QRectF itemrect = scene()->sceneRect();
    QRectF current = this->rect();
    qreal h = itemrect.height();
    qreal w = (itemrect.width() > current.width() ? itemrect.width() : current.width()) - 10;
    w = w > MINWIDTH ? w : MINWIDTH;

    scene()->setSceneRect(-w, -10, w, h);
    scene()->update();

    event->accept();
}

void GraphicArea::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

bool GraphicArea::eventFilter(QObject *obj, QEvent *event)
{
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
//                 if(this->currentRow() != 0)
//                     this->setCurrentItem(this->topLevelItem(this->currentRow() - 1));
             }
             break;

           case Qt::Key_Down:
             if (keyEvent->modifiers() & Qt::ControlModifier){
                 downAction();
             }else{
//                 if(this->currentRow() != 0)
//                     this->setCurrentItem(this->topLevelItem(this->currentRow() + 1));
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
    // standard event processing
//    qDebug() << event->type();
    return QObject::eventFilter(obj, event);
}

void GraphicArea::addItem(int id)
{
    setItem(id);
}

void GraphicArea::deleteItem(int id)
{

}

void GraphicArea::insertItem(int id)
{

}

void GraphicArea::swapItem(int before, int after)
{

}

void GraphicArea::replaceItem(int id)
{

}

void GraphicArea::popupAction()
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
    connect(m_add, &QAction::triggered, this, &GraphicArea::addAction);
    connect(m_delete, &QAction::triggered, this, &GraphicArea::deleteAction);
//    connect(m_edit, &QAction::triggered, this, &ProfileTreeWidget::editAction);
    connect(m_cut, &QAction::triggered, this, &GraphicArea::cutAction);
    connect(m_copy, &QAction::triggered, this, &GraphicArea::copyAction);
    connect(m_paste, &QAction::triggered, this, &GraphicArea::pasteAction);
    connect(m_up, &QAction::triggered, this, &GraphicArea::upAction);
    connect(m_down, &QAction::triggered, this, &GraphicArea::downAction);
    connect(m_ref, &QAction::triggered, this, &GraphicArea::reloadAction);
}

void GraphicArea::setItem(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    BaseNode *node = new BaseNode(this);
//    node->setPos(0, 250*(itemid - 1));

    //reading failure
    if(!editop->read(itemid, list)){
        delete node;
        return;
    }

    //get type
    QString type = list->at(0).at(1);

    //type local
    if(type == "local"){
        delete node;
        return;
    }

    if(type == "info"){
//        node->setPath(QColor(120,120,120), 2, QColor(230,230,230));
        setInfoItem(node, list, 1);
    }
    if(type == "normal"){
//        node->setPath(QColor(44,70,94), 2, QColor(222,235,247));
        setNormalItem(node, list, 0);
    }
    if(type == "search"){
//        node->setPath(QColor(56,87,35), 2, QColor(226,240,217));
        setSearchItem(node, list, 0);
    }
    if(type == "script"){
//        node->setPath(QColor(132,12,12), 2, QColor(251,215,214));
        setExtraFuncItem(node, list, 0);
    }
    if(type == "other"){
//        node->setPath(QColor(132,60,12), 2, QColor(255,242,204));
        setOtherItem(node, list, 0);
    }
    if(type == "temp"){
        setTempItem(node, list);
    }

    qDebug()<< "graphicarea::setTreeItem";

    //add list
    nodelist->insertNode(node, itemid);

    //add showing
    nodelist->getNodeArrow(itemid).node->setNewPos(0, 150*itemid);
    scene()->addItem(nodelist->getNodeArrow(itemid).node);
    if(nodelist->getNodeArrow(itemid).arrow != nullptr){
        scene()->addItem(nodelist->getNodeArrow(itemid).arrow);
    }

    delete list;
}


///DEPENDS_XML
void GraphicArea::setTempItem(BaseNode *node, QList<QStringList> *list)
{
    int istack = ((QString)list->at(1).at(1)).toInt();

    QHash<int, int> hlist;
    xgen.getListStructure(list, &hlist);

    switch (istack) {
    case XmlListGenerator::NORMAL:
        setNormalItem(node, list, hlist.value(XmlListGenerator::NORMAL));
        break;

    case XmlListGenerator::SEARCH:
        setSearchItem(node, list, hlist.value(XmlListGenerator::NORMAL));
        break;

    case XmlListGenerator::EXTRAFUNC:
        setExtraFuncItem(node, list, hlist.value(XmlListGenerator::EXTRAFUNC));
        break;

    case XmlListGenerator::OTHER:
        setOtherItem(node, list, hlist.value(XmlListGenerator::OTHER));
        break;

    default:
        break;
    }
}

///DEPENDS_XML
void GraphicArea::setInfoItem(BaseNode *node, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "(no name)" : curdata;
    node->addLines(QStringList() << "info" << "-----" << curdata);
    node->setPath(QColor(120,120,120), 2, QColor(230,230,230));
}

///DEPENDS_XML
void GraphicArea::setNormalItem(BaseNode *node, QList<QStringList> *list, int firstpos)
{
    int cmdskip = ((QString)list->at(firstpos + 2).at(1)).toInt();

    QStringList shlist;
    QString curdata;

    shlist.append("Execution");
    curdata = (cmdskip == 0)? "NewCommand" : list->at(firstpos + 3).at(1);
    shlist.append("-----");

    QFileInfo info(curdata);
    if(info.isFile()){
        shlist.append(info.fileName());
    }else{
        shlist.append(curdata);
    }

    for(int i = 1; i < cmdskip; i++){
        shlist.append(/*list->at(firstpos + 2 + i).at(3)
                      + ": "
                      + */list->at(firstpos + 3 + i).at(1));
    }

    node->addLines(shlist);
    node->setPath(QColor(44,70,94), 2, QColor(222,235,247));
}


///DEPENDS_XML
void GraphicArea::setSearchItem(BaseNode *node, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    curdata = list->at(firstpos + 1).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    node->addLines(QStringList() << "Search"
                   << "-----"
                   << curdata
                   << list->at(firstpos + 2).at(1)
                   << list->at(firstpos + 3).at(1));
    node->setPath(QColor(56,87,35), 2, QColor(226,240,217));
}


///DEPENDS_XML
void GraphicArea::setExtraFuncItem(BaseNode *node, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    QStringList shlist;
    int scrskip = ((QString)list->at(firstpos + 3).at(1)).toInt();

    shlist.append("Script");
    shlist.append("-----");

    curdata = list->at(firstpos + 1).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;

    QFileInfo info(curdata);
    if(info.isFile()){
        shlist.append(info.fileName());
    }else{
        shlist.append(curdata);
    }
    for(int i = 0; i < scrskip; i++){
        shlist.append(list->at(firstpos + 4 + i).at(1));
//        shlist.append(list->at(firstpos + 3 + i).at(3));
    }

    node->addLines(shlist);
    node->setPath(QColor(132,12,12), 2, QColor(251,215,214));
}


///DEPENDS_XML
void GraphicArea::setOtherItem(BaseNode *node, QList<QStringList> *list, int firstpos)
{
    QString curdata;
    curdata = list->at(firstpos + 1).at(1);
    curdata = (curdata == "")? "Unknown" : curdata;
    node->addLines(QStringList() << "Other"
                   << "-----"
                   << curdata);
    node->setPath(QColor(132,60,12), 2, QColor(255,242,204));
}

//void GraphicArea::scaleView(qreal scaleFactor)
//{
//    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
//    if (factor < 0.07 || factor > 100)
//        return;

//    scale(scaleFactor, scaleFactor);
//}
