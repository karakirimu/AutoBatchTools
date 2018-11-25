#ifndef NODEARROWLIST_H
#define NODEARROWLIST_H

#include <QGraphicsScene>
//#include <QObject>
#include "basenode.h"

//first or last item arrow is null
class BaseNode;
class Arrow;

class NodeArrow {
public:
    BaseNode *node;
    Arrow *arrow;
};

class NodeArrowList : public QObject
{
    Q_OBJECT
public:
    explicit NodeArrowList(QObject *parent = nullptr);
    ~NodeArrowList();

    //input functions
    void addNode(BaseNode *node);
    void insertNode(BaseNode *node, int index);
    void deleteNode(int index);
    void moveNode(int dest, int source);

    //output functions
    BaseNode *getNode(int index);
    NodeArrow getNodeArrow(int index);
    int nodecount();


    //reset functions
    void clearlist();

//    QGraphicsScene *getScene() const;
//    void setScene(QGraphicsScene *value);

//signals:
//    void structureChanged();

    int getCurrentIndex() const;
public slots:
    //    void updateScene();
    void setCurrentIndex(int value);

private:
    QList<NodeArrow> nodearrow;
    int currentIndex;
//    QGraphicsScene *scene;
};

#endif // NODEARROWLIST_H
