#ifndef STARTUPTABLE_H
#define STARTUPTABLE_H

//#include "optiontable.h"
#include <../basictable/basictable.h>
#include "startupdialog.h"
#include "startupxmlbuilder.h"

class StartupTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StartupTable(QWidget *parent = Q_NULLPTR);
    ~StartupTable();

private slots:
    void addAction();
    void editAction();
    void deleteAction();
    void reloadAction();
    void copyAction();
    void upAction();
    void downAction();
    void enableAction();
    void disableAction();

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event);

    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;
    QAction *m_enable;
    QAction *m_disable;

    StartupXmlBuilder *builder;
    void setTableItem(int row);
};

#endif // STARTUPTABLE_H
