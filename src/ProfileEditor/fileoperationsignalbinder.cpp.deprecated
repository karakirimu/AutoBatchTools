#include "fileoperationsignalbinder.h"

FileOperationSignalBinder::FileOperationSignalBinder(PESharedFunction *function, QObject *parent)
    : QObject(parent)
{
    func = function;
}

FileOperationSignalBinder::~FileOperationSignalBinder()
{
}

int FileOperationSignalBinder::itemCount()
{
    return func->itemCount();
}

bool FileOperationSignalBinder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return func->readItem(itemid, itemlist);
}

int FileOperationSignalBinder::firstPosNormal() const
{
    return func->firstPosNormal();
}

int FileOperationSignalBinder::firstPosSearch() const
{
    return func->firstPosSearch();
}

int FileOperationSignalBinder::firstPosScript() const
{
    return func->firstPosScript();
}

int FileOperationSignalBinder::firstPosOther() const
{
    return func->firstPosOther();
}

//void WidgetsSignalBinder::setTreeWidget(ProfileTreeWidget *widget)
//{
//    tree = widget;
//}

//void WidgetsSignalBinder::setGraphicAreaWidget(GraphicArea *area)
//{
//    graphic = area;
//}

void FileOperationSignalBinder::addItem()
{
    func->addItem();
    emit refreshFinished();
}

void FileOperationSignalBinder::insertItem(int index)
{
    func->addItem();
    int count = func->itemCount();
    for(int i = count;  i > index; i--){
        func->upItem(i);
    }

    emit refreshFinished();
}

void FileOperationSignalBinder::deleteItem(int index)
{
    func->deleteItem(index);
    emit refreshFinished();
}

void FileOperationSignalBinder::copyItem(int index)
{
    func->copyItem(index);
    emit refreshFinished();
}

//void WidgetsSignalBinder::editRead(int index, QList<QStringList> *indexlist)
//{
//    func->readItem(index, indexlist);
//    emit refreshFinished();
//}

void FileOperationSignalBinder::editWrite(int index, QList<QStringList> *itemlist)
{
    func->editItem(index, itemlist);
    emit refreshFinished();
}

//void WidgetsSignalBinder::replace(int index, bool isup)
//{
//    //if it is true, xml is to up that position
//    if(isup){
//        func->upItem(index);
//    }else{
//        func->downItem(index);
//    }
//    emit refreshFinished();
//}

void FileOperationSignalBinder::upItem(int index)
{
    func->upItem(index);
    emit refreshFinished();
}

void FileOperationSignalBinder::downItem(int index)
{
    func->downItem(index);
    emit refreshFinished();
}
