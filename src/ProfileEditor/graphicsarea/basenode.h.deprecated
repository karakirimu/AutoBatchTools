#ifndef BASENODE_H
#define BASENODE_H

#include "arrow.h"
#include "graphicarea.h"
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QObject>

class Arrow;
class GraphicArea;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class BaseNode : public QGraphicsItem
{
//    Q_OBJECT
public:
    BaseNode(GraphicArea *graphicarea);

    void addArrow(Arrow *arrow);
    QList<Arrow *> arrows() const;
    void addLines(QStringList text);
    QStringList text() const;

    //set rectanglepath
    void setPath(Qt::GlobalColor pencolor, int framesize, Qt::GlobalColor fillcolor);
    void setPath(QColor pencolor, int framesize, QColor fillcolor);

    //set framewidth and height
    void setWidth(qreal width);
    void setHeight(qreal height);

    //set new pos
    void setNewPos(qreal x, qreal y);

    //set nodeindex
    int getNodeIndex() const;
    void setNodeIndex(int index);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void itemMoving();
    bool advance();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

//signals:
//    void selectionChanged(int index);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<Arrow *> arrowList;
    QStringList textlines;
    QPointF newPos;
    int nodeIndex;
    GraphicArea *area;
    QPainterPath path;
    QPen pen;
    QColor fillcolor;
    qreal width;
    qreal height;
};

#endif // BASENODE_H
