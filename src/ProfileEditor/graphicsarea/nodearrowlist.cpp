#include "nodearrowlist.h"

NodeArrowList::NodeArrowList(QObject *parent)
    : QObject(parent)
{
//    setScene(nullptr);
}

NodeArrowList::~NodeArrowList()
{

}

void NodeArrowList::addNode(BaseNode *node)
{
    int count = nodearrow.count();
    NodeArrow na;
    node->setNodeIndex(count);
//    connect(node, SIGNAL(selectionChanged(int)), this, SLOT(setCurrentIndex(int)));
    na.node = node;

    if(count > 0){
        na.arrow = new Arrow(nodearrow.at(count - 1).node, node);
    }else{
        na.arrow = nullptr;
    }
    nodearrow.append(na);
//    updateScene();
}

void NodeArrowList::insertNode(BaseNode *node, int index)
{
    int count = nodearrow.count();
    if(count - 1 == index) addNode(node);

    //replace previous arrow
    NodeArrow previous = nodearrow.at(index);

    //avoid memory leak
    delete previous.arrow;
    previous.arrow = nullptr;

    //set new arrow data
    previous.arrow = new Arrow(node, nodearrow.at(index).node);

    nodearrow.replace(index, previous);

    NodeArrow na;
    node->setNodeIndex(index);
//    connect(node, SIGNAL(selectionChanged(int)), this, SLOT(setCurrentIndex(int)));
    na.node = node;
    na.arrow = new Arrow(nodearrow.at(index).node, node);

    nodearrow.insert(index, na);
    count = nodearrow.count();

    //update nodeindex
    for(int i = index + 1; i < count; i++){
        nodearrow.at(i).node->setNodeIndex(i);
    }

//    updateScene();
}

void NodeArrowList::deleteNode(int index)
{
    int count = nodearrow.count();
    if(count - 1 != index){
        NodeArrow previous = nodearrow.at(index + 1);

        //avoid memory leak
        delete previous.arrow;
        previous.arrow = nullptr;

        previous.arrow = new Arrow(nodearrow.at(index).node, previous.node);
        nodearrow.replace(index + 1, previous);
    }

    //delete internal data
//    disconnect(nodearrow.at(index).node, SIGNAL(selectionChanged(int)), this, SLOT(setCurrentIndex(int)));
    delete nodearrow.at(index).node;
    delete nodearrow.at(index).arrow;

    nodearrow.removeAt(index);

    //update nodeindex
    for(int i = index + 1; i < count; i++){
        nodearrow.at(i).node->setNodeIndex(i);
    }
//    updateScene();
}

void NodeArrowList::moveNode(int dest, int source)
{
    NodeArrow destnode = nodearrow.at(dest);
    NodeArrow sourcenode = nodearrow.at(source);
    BaseNode *tmp = destnode.node;
    tmp->setNodeIndex(source);
    sourcenode.node->setNodeIndex(dest);
    destnode.node = sourcenode.node;
    sourcenode.node = tmp;

    nodearrow.replace(dest, destnode);
    nodearrow.replace(source, sourcenode);
//    updateScene();
}

BaseNode *NodeArrowList::getNode(int index)
{
    return nodearrow.at(index).node;
}

NodeArrow NodeArrowList::getNodeArrow(int index)
{
//    qDebug() << "getNodeArrow : " << index;
    return nodearrow.at(index);
}

int NodeArrowList::nodecount()
{
    return nodearrow.count();
}

void NodeArrowList::clearlist()
{
    foreach(NodeArrow arr , nodearrow){
        delete arr.node;
        delete arr.arrow;
    }
    nodearrow.clear();
}

int NodeArrowList::getCurrentIndex() const
{
    return currentIndex;
}

void NodeArrowList::setCurrentIndex(int value)
{
    currentIndex = value;
}

//void NodeArrowList::updateScene()
//{
//    if(scene == nullptr) return;

//    int counter = nodearrow.count();
//    scene->clear();
//    scene->setSceneRect(-250, -10,500, 150 * counter);

//    for(int i = 0; i < counter; i++){
//        nodearrow.at(i).node->setNewPos(0, 150 * i);
//        scene->addItem(nodearrow.at(i).node);
//        if(nodearrow.at(i).arrow != nullptr){
//            scene->addItem(nodearrow.at(i).arrow);
//        }
//    }
//}

//QGraphicsScene *NodeArrowList::getScene() const
//{
//    return scene;
//}

//void NodeArrowList::setScene(QGraphicsScene *value)
//{
//    scene = value;
//}
