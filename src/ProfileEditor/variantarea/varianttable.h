#ifndef VARIANTTABLE_H
#define VARIANTTABLE_H

#include <QObject>
#include <QWidget>
#include <editoperator.h>
#include <xmllistgenerator.h>
//#include <optiontable.h>
#include <../basictable/basictable.h>
//#include <pesharedfunction.h>

class VariantTable : public BasicTable
{
    Q_OBJECT
public:
    explicit VariantTable(QWidget *parent = Q_NULLPTR);
    ~VariantTable();

//    void setSharedFunction(PESharedFunction *func);
    void setEditOperator(EditOperator *op);
public slots:
    void addAction();
    void editAction();
    void deleteAction();
    void copyAction();
    void upAction();
    void downAction();
    void reloadAction();

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event);
    bool setLocalListItem(int itemid);
    void tableItemSwap(int from , int to);

    void getLocalList(QList<QStringList> *newlist);

//    PESharedFunction *sfunction;
    EditOperator *editop;
    XmlListGenerator xgen;

    QAction *m_new;
    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    static const int MAGIC = 1;
};

#endif // VARIANTTABLE_H
