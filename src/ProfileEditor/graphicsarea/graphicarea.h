#ifndef GRAPHICSAREA_H
#define GRAPHICSAREA_H

#include "basenode.h"
#include "nodearrowlist.h"

#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <fileoperationsignalbinder.h>

#define MINWIDTH 400

class BaseNode;
class FileOperationSignalBinder;
class NodeArrowList;

class GraphicArea : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicArea(QWidget *parent = Q_NULLPTR);
    ~GraphicArea();

    void itemMoved();
    void itemSelectChanged(int index);

    void setWidgetsSignalBinder(FileOperationSignalBinder *bind);
signals:
    void data_add();
    void data_insert(int index);
    void data_delete(int index);
    void data_replace(int index, bool isup);
    void data_editread(int index, QList<QStringList> *indexlist);
    void data_editwrite(int index, QList<QStringList> *itemlist);

    void selectChangedAction(int index);

public slots:
    void zoomIn();
    void zoomOut();

    void addAction();
//    void editAction(int itemid, QList<QStringList> *itemlist);
    void deleteAction();
    void copyAction();
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

private:
    void popupAction();
    int currentRow();

    void setTreeItem(int itemid);
    void setTempTreeItem(BaseNode *node, QList<QStringList> *list);
    void setInfoTree(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setNormalTree(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setSearchTree(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setScriptTree(BaseNode *node, QList<QStringList> *list, int firstpos);
    void setOtherTree(BaseNode *node, QList<QStringList> *list, int firstpos);

    int timerId;
    QMenu *contextMenu;
    QAction *m_add;
//    QAction *m_edit;
    QAction *m_delete;
    QAction *m_copy;
    QAction *m_up;
    QAction *m_down;
    QAction *m_ref;

    BaseNode *centerNode;
    FileOperationSignalBinder *binder;
    NodeArrowList *nodelist;

    int selectedIndex;
};

#endif // GRAPHICSAREA_H
