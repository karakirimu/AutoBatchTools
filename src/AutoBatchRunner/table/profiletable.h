#ifndef PROFILETABLE_H
#define PROFILETABLE_H

//#include "optiontable.h"
#include <QProcess>
#include <../basictable/basictable.h>
//#include "profileeditor.h"
#include <../profilexmlbuilder/profilexmlbuilder.h>
#include <../processxmlbuilder/processxmlbuilder.h>

class ProfileTable : public BasicTable
{
    Q_OBJECT
public:
    explicit ProfileTable(QWidget *parent = Q_NULLPTR);
    ~ProfileTable();

public slots:
    void newAction();
    void addAction();
    void editAction();
    void deleteAction();
    void upAction();
    void downAction();
    void reloadAction();
private:
    void setPopupActionTop();
    bool eventFilter(QObject *obj, QEvent *event);
    void createList(QString filename, QList<QStringList> *newlist);

    QAction *m_new;
    QAction *m_add;
    QAction *m_edit;
    QAction *m_delete;
    QAction *m_ref;

    ProfileXmlBuilder *builder;
    void setTableItem(int row);
};

#endif // PROFILETABLE_H
