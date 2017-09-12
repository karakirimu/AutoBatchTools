#ifndef FILEQUEUETABLE_H
#define FILEQUEUETABLE_H

#include "filequeuetable_global.h"
#include "../basictable/basictable.h"
#include "../fileinfodialog/fileinfodialog.h"

class FILEQUEUETABLESHARED_EXPORT FileQueueTable : public BasicTable
{
    Q_OBJECT
public:
    FILEQUEUETABLESHARED_EXPORT explicit FileQueueTable(QWidget *parent = nullptr);
    FILEQUEUETABLESHARED_EXPORT ~FileQueueTable();

    FILEQUEUETABLESHARED_EXPORT void addFiles(const QStringList &filenames);

protected slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void setPopupActionTop();
    void setPopupActionDefault();
    void setPopupActionBottom();

    void addAction();
    void horizontalHeaderClicked(int column);
    void deleteAciton();
    void clearAction();
    void propertyAction();

private:
    QAction *m_add;
    QAction *m_deleteitems;
    QAction *m_deleteAll;
    QAction *m_property;
};

#endif // FILEQUEUETABLE_H
