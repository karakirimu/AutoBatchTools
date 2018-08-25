#ifndef FLOWTABLE_H
#define FLOWTABLE_H

#include <../basictable/basictable.h>
#include <editoperator.h>
#include <flowcellwidget.h>

class FlowTable : public BasicTable
{
    Q_OBJECT
public:
    explicit FlowTable(QWidget *parent = nullptr);
    ~FlowTable();

    void setEditOperator(EditOperator *op);

    void reloadAction();

private slots:
    bool eventFilter(QObject *obj, QEvent *event);

    //for data action
    void addAction();
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();

    //for inner item set
    //syncronize with Item
    void addItem();
    void deleteItem(int id);
    void insertItem(int id);
    void swapItem(int before, int after);
    void replaceItem(int id);

    //table selection changed
    void selectChanged(int crow, int ccol, int prow, int pcol);

    void onItemStatusChanged(int after, int before, int function, int sendfrom);

private:
//    void mousePressEvent(QMouseEvent *event);
    int fixedCurrentRow();
    int fixedRowFromId(int id);

    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    void setFlowItem(int itemid, bool selector);
    void setAllFlowItem();
    void setTempItem(FlowCellWidget *cell, QList<QStringList> *list);
    void setInfoItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setNormalItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setSearchItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setPluginsItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setOtherItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);

    void updateLastIndexItem(int lastindex);

//    QString getHtmlHeader(QString headcssstr);
//    QString getHtmlFooter();

    QAction *m_add;
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
    const QString info_title = "Information";
    const QPixmap info_pixmap = QIcon(":/default_icons/info.png").pixmap(16,16);
    const QString info_style = "color: black; background-color: #e8e8e8;";
    const QString info_frame = "background-color: #606060;";

    const QString exec_title = "Executable";
    const QPixmap exec_pixmap = QIcon(":/default_icons/terminal.png").pixmap(16,16);
    const QString exec_style = "color: black; background-color: #e2f6ff;";
    const QString exec_frame = "background-color: #b0c3cc;";

    const QString search_title = "File Search";
    const QPixmap search_pixmap = QIcon(":/default_icons/search.png").pixmap(16,16);
    const QString search_style = "color: black; background-color: #dcedc8;";
    const QString search_frame = "background-color: #aabb97;";

    const QString extra_title = "External Plugins";
    const QPixmap extra_pixmap = QIcon(":/default_icons/extras.png").pixmap(16,16);
    const QString extra_style = "color: black; background-color: #ffcdd2;";
    const QString extra_frame = "background-color: #cb9ca1;";

    const QString other_title = "Other Projects";
    const QPixmap other_pixmap = QIcon(":/default_icons/others.png").pixmap(16,16);
    const QString other_style = "color: black; background-color: #ffecb3;";
    const QString other_frame = "background-color: #cbba83;";
};

#endif // FLOWTABLE_H
