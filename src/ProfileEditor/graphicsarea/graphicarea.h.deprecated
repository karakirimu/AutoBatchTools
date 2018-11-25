#ifndef GRAPHICSAREA_H
#define GRAPHICSAREA_H

#include "basenode.h"
#include "nodearrowlist.h"

#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <editoperator.h>
//#include <fileoperationsignalbinder.h>
#include <xmllistgenerator.h>

#define MINWIDTH 400

class BaseNode;
//class FileOperationSignalBinder;
class NodeArrowList;

class GraphicArea : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicArea(QWidget *parent = Q_NULLPTR);
    ~GraphicArea();

    void itemMoved();
    void itemSelectChanged(int index);

//    void setWidgetsSignalBinder(FileOperationSignalBinder *bind);
    void setEditOperator(EditOperator *op);
signals:
//    void data_add();
//    void data_insert(int index);
//    void data_delete(int index);
//    void data_replace(int index, bool isup);
//    void data_editread(int index, QList<QStringList> *indexlist);
//    void data_editwrite(int index, QList<QStringList> *itemlist);

    void selectChangedAction(int index);

public slots:
    void zoomIn();
    void zoomOut();

    void addAction();
//    void editAction(int itemid, QList<QStringList> *itemlist);
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();
    void reloadAction();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
//#ifndef QT_NO_WHEELEVENT
//    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
//#endif
//    void scaleView(qreal scaleFactor);

    void resizeEvent(QResizeEvent *event);

private slots:
    void onCustomContextMenu(const QPoint &point);
    bool eventFilter(QObject *obj, QEvent *event);

    //syncronize with Item
    void addItem(int id);
    void deleteItem(int id);
    void insertItem(int id);
    void swapItem(int before, int after);
    void replaceItem(int id);

private:
    void popupAction();
    int currentRow();

    void setItem(int itemid);
    void setTempItem(BaseNode *node, QList<QStringList> *list);
    void setInfoItem(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setNormalItem(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setSearchItem(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setExtraFuncItem(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setOtherItem(BaseNode *node, QList<QStringList> *list, int firstpos);

    int timerId;
    QMenu *contextMenu;
    QAction *m_add;
//    QAction *m_edit;
    QAction *m_delete;
    QAction *m_cut;
    QAction *m_copy;
    QAction *m_paste;
    QAction *m_up;
    QAction *m_down;
    QAction *m_ref;

    BaseNode *centerNode;
    NodeArrowList *nodelist;

//    FileOperationSignalBinder *binder;
    EditOperator *editop;
    XmlListGenerator xgen;

    int selectedIndex;
};

#endif // GRAPHICSAREA_H
