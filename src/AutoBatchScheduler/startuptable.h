#ifndef STARTUPTABLE_H
#define STARTUPTABLE_H

#include <../basictable/basictable.h>
#include "startupdialog.h"
#include "startupxmlbuilder.h"
#include "taskschedulerconnector.h"

class StartupTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StartupTable(QWidget *parent = nullptr);
    ~StartupTable();
    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

signals:
    //to systemtray ui
    void actionDeleted(QString);
    void actionAdded(int);

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

    void updateItemEnabled(QString objectname);

private:
    void setPopupActionTop();
    void setPopupActionBottom();
    bool eventFilter(QObject *obj, QEvent *event);
    int getStartupXmlIndex(QString objectname);

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
