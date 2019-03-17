#ifndef PLUGINSTREE_H
#define PLUGINSTREE_H

#include "pluginstree_global.h"
#include <QPluginLoader>
#include <QTreeWidget>
#include <QHeaderView>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QKeyEvent>
#include <../extrafunctionsxmlbuilder/extrafunctionsxmlbuilder.h>
#include <../plugins/ExtraPluginInterface/extraplugininterface.h>

#ifdef QT_DEBUG
    #include <QTime>
#endif

class PLUGINSTREESHARED_EXPORT PluginsTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PluginsTree(QWidget *parent = nullptr);
    ~PluginsTree();

    void reloadAction();

public slots:
    void addAction();
    void deleteAction();
    void upAction();
    void downAction();

private slots:
    void itemSelectUpdate();
    void itemDataUpdate(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    // undo redo no emit operation
    void insertRow(int row); //ui
    void insertRow(int row, QModelIndex *index); //internal function

    void deleteRowItems();

    void replaceItem(int row, QStringList strlist, int section);

    void swapRow(int before, int after);
    void swapRow(int before, int after, QModelIndex *index);

    // popup
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    // keyboard shortcut
    bool eventFilter(QObject *obj, QEvent *event);
    void onCustomContextMenu(const QPoint &point);

    // drag and drop
    void dropEvent(QDropEvent *event);

    bool setAutoDetectPlugins(QTreeWidgetItem *parent);
    bool setManualPlugins(QTreeWidgetItem *parent);

    QStringList getLocalRowElement(int targetrow, int tablerow);
    QStringList getGlobalRowElement(int row);
    QStringList getEmptyVariants();
    QStringList getVariants(QTreeWidgetItem *childitem);

    void closeEditState();

    bool isParentValid();
    QModelIndex getSectionFromUi();

    //copied from BasicTable
    bool deleteCheckMessage();

    //model()->datachanged
    //like blocksignals
    void ignoreDataChangedSignal(bool valid);

    ExtrafunctionsXmlBuilder *builder;

    QAction *m_add;
    QAction *m_delete;
    QAction *m_ref;
    QAction *m_up;
    QAction *m_down;

    QMenu *contextMenu;

    // order of root element
    enum{TREE_AUTO,TREE_MANUAL};
};

#endif // PLUGINSTREE_H
