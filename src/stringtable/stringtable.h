#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include "stringtable_global.h"
#include "../basictable/basictable.h"
#include "../stringxmlbuilder/stringxmlbuilder.h"

class STRINGTABLESHARED_EXPORT StringTable : public BasicTable
{
    Q_OBJECT
public:
    STRINGTABLESHARED_EXPORT explicit StringTable(QWidget *parent = Q_NULLPTR);
    STRINGTABLESHARED_EXPORT ~StringTable();

public slots:
    STRINGTABLESHARED_EXPORT void addAction();
    STRINGTABLESHARED_EXPORT void editAction();
    STRINGTABLESHARED_EXPORT void deleteAction();
    STRINGTABLESHARED_EXPORT void reloadAction();
    STRINGTABLESHARED_EXPORT void copyAction();
    STRINGTABLESHARED_EXPORT void upAction();
    STRINGTABLESHARED_EXPORT void downAction();
    STRINGTABLESHARED_EXPORT void openFileAction();
    STRINGTABLESHARED_EXPORT void openDirectoryAction();

    STRINGTABLESHARED_EXPORT void saveAction(int row);

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    void setTableItem(int row);
    bool eventFilter(QObject *obj, QEvent *event);
    void createList(int row, QList<QStringList> *newlist);

    QAction *m_file;
    QAction *m_dir;
    QAction *m_add;
    QAction *m_delete;
    QAction *m_edit;
    QAction *m_ref;

    StringXmlBuilder *builder;
};

#endif // STRINGTABLE_H
