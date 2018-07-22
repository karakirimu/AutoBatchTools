#ifndef EXTRAFUNCTIONSTABLE_H
#define EXTRAFUNCTIONSTABLE_H

#include "extrafunctionstable_global.h"
#include <QPluginLoader>
#include <../basictable/basictable.h>
#include <../extrafunctionsxmlbuilder/extrafunctionsxmlbuilder.h>
#include "../plugins/ExtraPluginInterface/extraplugininterface.h"

class EXTRAFUNCTIONSTABLESHARED_EXPORT ExtraFunctionsTable : public BasicTable
{
    Q_OBJECT
public:
    explicit ExtraFunctionsTable(QWidget *parent = nullptr);
    ~ExtraFunctionsTable();

public slots:
    void addAction();
    void editAction();
    void deleteAction();
    void reloadAction();
    void copyAction();
    void upAction();
    void downAction();
    void openFileAction();

    void saveAction(int row);

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    void setTableItem(int row);
    bool eventFilter(QObject *obj, QEvent *event);
    void createList(int row, QList<QStringList> *newlist);

    QAction *m_file;
    QAction *m_add;
    QAction *m_delete;
    QAction *m_edit;
    QAction *m_ref;

    ExtrafunctionsXmlBuilder *builder;
};

#endif // EXTRAFUNCTIONSTABLE_H
