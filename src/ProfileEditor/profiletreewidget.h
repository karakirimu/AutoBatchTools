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
#include <xmllistgenerator.h>
//#include <../xmlbuilder/localloader/profileeditorxmlbuilder.h>
//#include <pesharedfunction.h>
#include <../variantconverter/variantconverter.h>
//#include <fileoperationsignalbinder.h>

//class FileOperationSignalBinder;

class ProfileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ProfileTreeWidget(QWidget *parent = 0);
    ~ProfileTreeWidget();

    //for order "1". it becomes to be unique order.
    int fixedCurrentRow();
    int fixedRowFromId(int id);
    int rowFromItem(QTreeWidgetItem *item);
//    void setSharedFunction(PESharedFunction *func);
//    void setWidgetsSignalBinder(FileOperationSignalBinder *bind);
    void setEditOperator(EditOperator *op);

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

//    void readItem(int itemid, QList<QStringList> *itemlist);
//    void saveAction();
//    void saveOverWriteAction();

//    void saveCurrentAction();
signals:
    void editActionKeyPressed();
//    void indexChanged(int);

//    void data_add();
//    void data_insert(int index);
//    void data_delete(int index);
//    void data_replace(int index, bool isup);
//    void data_editread(int index, QList<QStringList> *indexlist);
//    void data_editwrite(int index, QList<QStringList> *itemlist);

private slots:
    void rowSelected();
    void onCustomContextMenu(const QPoint &point);
    void onItemStatusChanged(int after, int before, int function, int sendfrom);
    bool eventFilter(QObject *obj, QEvent *event);

    //tree data synchronize
    void addTree(int id);
    void deleteTree(int id);
    void insertTree(int id);
    void swapTree(int before, int after);
    void replaceTree(int id);

private:
    void popupAction();
    int currentRow();

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
    XmlListGenerator xgen;
//    ProfileEditorXmlBuilder *builder;
//    PESharedFunction *sfunction;
//    FileOperationSignalBinder *binder;
//    QString tmp;
//    QString currentfile;
};

#endif // PROFILETREEWIDGET_H
