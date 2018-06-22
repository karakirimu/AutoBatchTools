#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include "stringtable_global.h"
#include "../basictable/basictable.h"
#include "../stringxmlbuilder/stringxmlbuilder.h"
#include <QClipboard>
#include <QApplication>

class STRINGTABLESHARED_EXPORT StringTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StringTable(QWidget *parent = Q_NULLPTR);
    ~StringTable();

public slots:
    void addAction();
    void editAction();
    void deleteAction();
    void reloadAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();
    void openFileAction();
    void openDirectoryAction();

    void saveAction(int row);
    void resave();

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();
    void setTableItem(int row);
    bool eventFilter(QObject *obj, QEvent *event);
    void createList(int row, QList<QStringList> *newlist);

    QAction *m_file;
    QAction *m_dir;
    QAction *m_add;
    QAction *m_delete;
    QAction *m_edit;
    QAction *m_cut;
    QAction *m_paste;
    QAction *m_ref;

    StringXmlBuilder *builder;
};

#endif // STRINGTABLE_H
