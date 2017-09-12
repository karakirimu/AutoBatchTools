#include "basenode.h"

BaseNode::BaseNode(GraphicArea *graphicarea)
    : area(graphicarea)
{
    setFlags(ItemSendsGeometryChanges | ItemIsMovable | ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setWidth(300);
    setHeight(100);
}

void BaseNode::addArrow(Arrow *arrow)
{
    arrowList << arrow;
    arrow->adjust();
}

QList<Arrow *> BaseNode::arrows() const
{
    return arrowList;
}

void BaseNode::addLines(QStringList text)
{
    this->textlines.append(text);
}

QStringList BaseNode::text() const
{
    return textlines;
}

void BaseNode::setPath(Qt::GlobalColor pencolor, int framesize, Qt::GlobalColor fillcolor)
{
    QPen dpen(pencolor, framesize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen = dpen;
    int adjust = framesize * 2;
    path.addRoundedRect(-(width/2) - framesize -0.5, -framesize -0.5, width + adjust, height + adjust, 10, 10);
    this->fillcolor = QColor(fillcolor);
}

void BaseNode::setPath(QColor pencolor, int framesize, QColor fillcolor)
{
    QPen dpen(pencolor, framesize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen = dpen;
    int adjust = framesize * 2;
    path.addRoundedRect(-(width/2) - framesize -0.5, -framesize -0.5, width + adjust, height + adjust, 10, 10);
    this->fillcolor = fillcolor;
}

void BaseNode::setWidth(qreal width)
{
    this->width = width;
}

void BaseNode::setHeight(qreal height)
{
    this->height = height;
}

void BaseNode::setNewPos(qreal x, qreal y)
{
    setPos(x, y);
    newPos = pos();
}

void BaseNode::itemMoving()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }
//! [2]

////! [3]
//    // Sum up all forces pushing this item away
//    qreal xvel = 0;
//    qreal yvel = 0;
//    foreach (QGraphicsItem *item, scene()->items()) {
//        BaseNode *node = qgraphicsitem_cast<BaseNode *>(item);
//        if (!node)
//            continue;

//        QPointF vec = mapToItem(node, 0, 0);
//        qreal dx = vec.x();
//        qreal dy = vec.y();
//        double l = 2.0 * (dx * dx + dy * dy);
//        if (l > 0) {
//            xvel += (dx * 150.0) / l;
//            yvel += (dy * 150.0) / l;
//        }
//    }
////! [3]

//! [4]
//     Now subtract all forces pulling items together
//    double weight = (arrowList.size() + 1) * 10;
//    foreach (Arrow *arrow, arrowList) {
//        QPointF vec;
//        if (arrow->sourceNode() == this)
//            vec = mapToItem(arrow->destNode(), 0, 0);
//        else
//            vec = mapToItem(arrow->sourceNode(), 0, 0);
//            xvel -= vec.x() / weight;
//            yvel -= vec.y() / weight;
//    }
//! [4]

////! [5]
//    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
//        xvel = yvel = 0;
////! [5]

////! [6]
//    QRectF sceneRect = scene()->sceneRect();
//    newPos = pos() + QPointF(xvel, yvel);
//    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 100), sceneRect.right() - 100));
//    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 100), sceneRect.bottom() - 100));

}

bool BaseNode::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF BaseNode::boundingRect() const
{
    return path.boundingRect();
}

QPainterPath BaseNode::shape() const
{
    return path;
}

void BaseNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);

//    qDebug() << "painter option : " << option->state;

    if (option->state & QStyle::State_Sunken) {
        QPen penc(pen.color().darker(), pen.widthF(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(penc);
        painter->fillPath(path, fillcolor.darker());
        painter->drawPath(path);

    } else if(option->state & QStyle::State_Selected){
        QPen penc(pen.color().lighter(), pen.widthF(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(penc);
        painter->fillPath(path, fillcolor.lighter());
        painter->drawPath(path);

    } else{
        painter->setPen(pen);
        painter->fillPath(path, fillcolor);
        painter->drawPath(path);

    }

    //add abstract text
    QFont font("Times", 10);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setPen(Qt::black);
    painter->setFont(font);
    painter->save();
    painter->scale(1, 1);
    int i = 20;
    foreach (QString line, textlines) {
        painter->drawText(-(width/2) + 10, i, line);
        i+=10;
    }

    painter->restore();
}

QVariant BaseNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Arrow *arrow, arrowList)
            arrow->adjust();
        area->itemMoved();
        break;

    case ItemSelectedChange:
        if(!this->isSelected()){
            qDebug() << ":: itemselectedchange :: isSelected : "
                     << this->isSelected()
                     << " : "
                     << this->getNodeIndex();
//            emit selectionChanged(this->getNodeIndex());
            area->selectChangedAction(this->getNodeIndex());


        }
        break;

    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void BaseNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void BaseNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

int BaseNode::getNodeIndex() const
{
    return nodeIndex;
}

void BaseNode::setNodeIndex(int index)
{
    nodeIndex = index;
}
