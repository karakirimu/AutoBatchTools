#ifndef PLUGINSTREE_H
#define PLUGINSTREE_H

#include "pluginstree_global.h"
#include <QPluginLoader>
#include <QFileDialog>
#include <QTreeWidget>
#include <QHeaderView>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QKeyEvent>
#include <../filesearchloader/filesearchloader.h>
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

private:
    // undo redo no emit operation
    void insertRow(int row, QStringList *data); //ui
    void insertRow(int row, QStringList *data, QModelIndex *index); //internal function

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
    bool setAutoDetectPlugins(QTreeWidgetItem *parent);
    bool setManualPlugins(QTreeWidgetItem *parent);

    // unique variant of xml files
    QStringList getEmptyVariants();
    QStringList getVariants(QTreeWidgetItem *childitem);
    QList<QStringList> createXmlVariants(QStringList *uitext);

    //load plugin checking and return ui texts
    QStringList loadPluginUiText(const QStringList *xmltext);
    bool isPluginValid(const QStringList *xmltext);

    //for tree ui management
    void closeEditState();

    bool isParentValid();
    QModelIndex getSectionFromUi();

    //copied from BasicTable
    bool deleteCheckMessage();

    //file open (copy of basetable::selectfiles)
    QStringList selectFiles(QString basedir);

    ExtrafunctionsXmlBuilder *builder;

    QAction *m_add;
    QAction *m_delete;
    QAction *m_ref;
    QAction *m_up;
    QAction *m_down;

    QMenu *contextMenu;

    // order of root element
    enum{TREE_AUTO,TREE_MANUAL};

    // An order of QStringList for ui
    enum{NAME_UI,VERSION_UI,VENDOR_UI,PATH_UI,DESC_UI};

    const int NAME_XML = 1;
    const int PATH_XML = 3;
};

#endif // PLUGINSTREE_H
