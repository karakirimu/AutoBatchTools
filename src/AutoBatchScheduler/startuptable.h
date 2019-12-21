#ifndef STARTUPTABLE_H
#define STARTUPTABLE_H

#include <../basictable/basictable.h>
#include "startupdialog.h"
#include "startupxmlbuilder.h"
#include "taskschedulerconnector.h"
#include <../variantconverter/variantconverter.h>

#include <QSettings>

class StartupTable : public BasicTable
{
    Q_OBJECT
public:
    explicit StartupTable(QWidget *parent = nullptr);
    ~StartupTable();
    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

public slots:
    void reloadAction();

private slots:
    void addAction();
    void editTableAction(int row, int col);
    void editAction();
    void deleteAction();
    void copyAction();
    void upAction();
    void downAction();
    void enableAction();
    void disableAction();

    void updateItemEnabled(QString objectname);

private:
    void setPopupActionTop();
    void setPopupActionDefault();
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
    TaskSchedulerConnector *taskc;
    void setTableItem(int row);
    void replaceItem(int row);
    QString getRandomString(int length);
};

#endif // STARTUPTABLE_H
