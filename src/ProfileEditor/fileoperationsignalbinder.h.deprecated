#ifndef WIDGETSSIGNALBINDER_H
#define WIDGETSSIGNALBINDER_H

//#include "profiletreewidget.h"

#include <QObject>
//#include <graphicarea.h>
#include <pesharedfunction.h>

//class ProfileTreeWidget;
//class GraphicArea;

class FileOperationSignalBinder : public QObject
{
    Q_OBJECT
public:
    explicit FileOperationSignalBinder(PESharedFunction *function, QObject *parent = nullptr);
    ~FileOperationSignalBinder();

//    void setTreeWidget(ProfileTreeWidget *widget);
//    void setGraphicAreaWidget(GraphicArea *area);

    int itemCount();
    bool readItem(int itemid, QList<QStringList> *itemlist);

    //inherit
    int firstPosNormal() const;
    int firstPosSearch() const;
    int firstPosScript() const;
    int firstPosOther() const;

signals:
    void refreshFinished();

public slots:
    void addItem();
    void insertItem(int index);
    void deleteItem(int index);
    void copyItem(int index);
//    void replace(int index, bool isup);
    void upItem(int index);
    void downItem(int index);
//    void editRead(int index, QList<QStringList> *indexlist);
    void editWrite(int index, QList<QStringList> *itemlist);

private:
    PESharedFunction *func;
//    ProfileTreeWidget *tree;
//    GraphicArea *graphic;
};

#endif // WIDGETSSIGNALBINDER_H
