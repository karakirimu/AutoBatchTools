#ifndef EXTRAFUNCTIONSTABLE_H
#define EXTRAFUNCTIONSTABLE_H

#include "extrafunctionstable_global.h"
#include <QPluginLoader>
#include <../basictable/basictable.h>
#include <../extrafunctionsxmlbuilder/extrafunctionsxmlbuilder.h>
#include "../plugins/RunnerExtraPluginInterface/runnerextraplugininterface.h"

class EXTRAFUNCTIONSTABLESHARED_EXPORT ExtraFunctionsTable : public BasicTable
{
    Q_OBJECT
public:
    EXTRAFUNCTIONSTABLESHARED_EXPORT explicit ExtraFunctionsTable(QWidget *parent = nullptr);
    EXTRAFUNCTIONSTABLESHARED_EXPORT ~ExtraFunctionsTable();

public slots:
    EXTRAFUNCTIONSTABLESHARED_EXPORT void addAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void editAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void deleteAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void reloadAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void copyAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void upAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void downAction();
    EXTRAFUNCTIONSTABLESHARED_EXPORT void openFileAction();

    EXTRAFUNCTIONSTABLESHARED_EXPORT void saveAction(int row);

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
