#ifndef ARROW_H
#define ARROW_H

#include "basenode.h"

#include <QGraphicsItem>
#include <QPainter>
//#include <QObject>
#include <math.h>

class BaseNode;

class Arrow : public QGraphicsItem
{
//    Q_OBJECT
public:
    Arrow(BaseNode *sourceNode, BaseNode *destNode);

    BaseNode *sourceNode() const;
    BaseNode *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE { return Type; }

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    BaseNode *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // ARROW_H
