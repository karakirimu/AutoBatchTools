#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

#include "searchtable_global.h"
#include "../basictable/basictable.h"
#include "../searchxmlbuilder/searchxmlbuilder.h"
#include "../filesearchdialog/filesearchdialog.h"

class SEARCHTABLESHARED_EXPORT SearchTable : public BasicTable
{
    Q_OBJECT
public:
    explicit SearchTable(QWidget *parent = nullptr);
    ~SearchTable();

public slots:
    void addAction();
    void editAction();
    void deleteAction();
    void reloadAction();
    void copyAction();
    void upAction();
    void downAction();

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event);

    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    SearchXmlBuilder *builder;
    void setTableItem(int row);
};

#endif // SEARCHTABLE_H
