#ifndef PROCESSSHOWTABLE_H
#define PROCESSSHOWTABLE_H

#include <QObject>
#include <QTableWidget>
#include <QWidget>
#include <QHeaderView>
#include <taskschedulerconnector.h>
#include "cellinfowidget.h"
#include "consolewidget.h"

class ProcessShowTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit ProcessShowTable(QWidget *parent = nullptr);
    ~ProcessShowTable();

    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

signals:
    //from cellwidget
    void pause(QString);
    void stop(QString);

    //to cellwidget
    void setProgress(QString, int, int);
    void updateProgress(QString, int);
    void updateProcess(QString, QString, int);

    //to systemtray
    void infoNofile(QString);

public slots:
    //create ui
//    void addItem(QString objname);
//    void removeItem(QString objectname);
    void insertItem(int itemid);
    void enableItem(QString objname);
    void disableItem(QString objname);

    //send command to parent
    void pauseClicked(QString objname){emit pause(objname);}
    void stopClicked(QString objname){emit stop(objname);}

    //receive command from parent
    void setProgressRange(QString objname, int start, int end);
    void updateProgressBar(QString objname, int num);
    void setUpdateProcess(QString objname, QString data, int type);

    void encounterdTime(QString objname);
    void timerStarted(QString objname, QDateTime date);
    void setProcessEnd(QString objname, int type);
    void setProcessStopped(QString objname);
    void setProcessError(QString objname, int currentnum);
    void setProcessErrorText(QString objectname, QString str);

    //launch in this class
    void launchConsole(QString objname);

private slots:
    void onCheckStateChanged(bool checked);

private:
    void initCellWidgets();
    void changeXmlValidState(int itemid);
    int getStartupXmlIndex(QString objectname);

    CellInfoWidget *getCellWidget(QString objname);

    StartupXmlBuilder *builder;
    TaskSchedulerConnector *taskc;

};

#endif // PROCESSSHOWTABLE_H
