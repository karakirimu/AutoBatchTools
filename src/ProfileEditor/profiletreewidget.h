#ifndef PROFILETREEWIDGET_H
#define PROFILETREEWIDGET_H

#include <QMouseEvent>
#include <QTreeWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QWidget>
#include <QObject>
#include <QMenu>

#include <editoperator.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class ProfileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ProfileTreeWidget(QWidget *parent = nullptr);
    ~ProfileTreeWidget();

    void setEditOperator(EditOperator *op);

    //format : %1,%2
    void updateIndex(QString operation);

public slots:
    void addAction();
//    void editAction(int itemid, QList<QStringList> *itemlist);
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();
    void reloadAction();

signals:
    void editActionKeyPressed();

private slots:
    void rowSelected();
    void onCustomContextMenu(const QPoint &point);
    void onItemStatusChanged(int after, int before, int function, int sendfrom);
    bool eventFilter(QObject *obj, QEvent *event);

    //tree synchronize with data (from data index)
    void addTree(int id);
    void deleteTree(int id);
    void insertTree(int id);
    void swapTree(int before, int after);
    void replaceTree(int id);

private:
    void popupAction();
    int currentRow();

    //from xml data to ui index
    int fixedCurrentRow();
    int dataToUiIndex(int id);

    //from ui index to xml data
    int uiIndexToData(int id);

    //tree set functions
    void setTree(int itemid);
    void setTempTree(QTreeWidgetItem *root, QList<QStringList> *list);
    void setInfoTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos);
    void setNormalTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos);
    void setSearchTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos);
    void setExtraFuncTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos);
    void setOtherTree(QTreeWidgetItem *root, QList<QStringList> *list, int firstpos);

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

    EditOperator *editop;
    ProcessXmlListGenerator xgen;

    //for reduce read time
    const QIcon exec_icon = QIcon(":/default_icons/terminal.png");
    const QIcon search_icon = QIcon(":/default_icons/search.png");
    const QIcon extra_icon = QIcon(":/default_icons/extras.png");
    const QIcon other_icon = QIcon(":/default_icons/others.png");

};

#endif // PROFILETREEWIDGET_H
