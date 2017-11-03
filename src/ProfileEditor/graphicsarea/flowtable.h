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
    void setTempItem(FlowCellWidget *cell, QList<QStringList> *list);
    void setInfoItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setNormalItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setSearchItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setExtraFuncItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);
    void setOtherItem(FlowCellWidget *cell, QList<QStringList> *list, int firstpos);

    void updateLastIndexItem(int lastindex);

    QString getHtmlHeader(QString headcssstr);
    QString getHtmlFooter();

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
};

#endif // FLOWTABLE_H
