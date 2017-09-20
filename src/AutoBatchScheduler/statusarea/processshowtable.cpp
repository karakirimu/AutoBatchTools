#include "processshowtable.h"

ProcessShowTable::ProcessShowTable(QWidget *parent)
 : QTableWidget(parent)
{
    setColumnCount(1);
    setRowCount(0);
    //adjust column
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //hide number
    horizontalHeader()->hide();
    verticalHeader()->hide();

    setEditTriggers(QTableWidget::NoEditTriggers);
}

ProcessShowTable::~ProcessShowTable()
{

}

void ProcessShowTable::addItem(QString objname)
{
    int count = this->rowCount();
    setRowCount(count + 1);
    CellInfoWidget *widget = new CellInfoWidget();
    widget->setObjectName(objname);

    //clicked operation
    connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
    connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
    connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

    //update operation
//    connect(this, &ProcessShowTable::updateProcess, widget, &CellInfoWidget::updateProcess);
//    connect(this, &ProcessShowTable::updateProgress, widget, &CellInfoWidget::updateProgress);

    //set action
//    connect(this, &ProcessShowTable::setProgress, widget, &CellInfoWidget::setProgressminmax);

    setRowHeight(count, widget->height());
    setCellWidget(count, 0, widget);
}

void ProcessShowTable::removeItem(QString objectname)
{
    //search object name
    int count = this->rowCount();
    int row = -1;
    QWidget *widget = nullptr;
    for(int i = 0; i < count; i++){
        widget = this->cellWidget(i,0);
        if(widget->objectName() == objectname){
            row = i;
            break;
        }
    }

    //remove pattern matched item
    if(widget != nullptr && row > -1){
        CellInfoWidget *ciw = qobject_cast<CellInfoWidget *>(widget);

        //clicked operation
        disconnect(ciw, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
        disconnect(ciw, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
        disconnect(ciw, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

        //update operation
//        disconnect(this, &ProcessShowTable::updateProcess, ciw, &CellInfoWidget::updateProcess);
//        disconnect(this, &ProcessShowTable::updateProgress, ciw, &CellInfoWidget::updateProgress);

        //set action
//        disconnect(this, &ProcessShowTable::setProgress, ciw, &CellInfoWidget::setProgressminmax);

        delete ciw;
        this->removeRow(row);
        setRowCount(count - 1);
    }
}

void ProcessShowTable::setProgressRange(QString objname, int start, int end)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->setProgressminmax(start, end);
}

void ProcessShowTable::updateProgressBar(QString objname, int num)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->updateProgress(num);
}

void ProcessShowTable::setUpdateProcess(QString objname, QString data, int type)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->updateProcess(data, type);
}

void ProcessShowTable::encounterdTime(QString objname)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->started();
}

void ProcessShowTable::timerStarted(QString objname, QDateTime date)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->setTimerEnd(date.toString("yyyy/MM/dd HH:mm:ss"));
}

void ProcessShowTable::setProcessEnd(QString objname, int type)
{
    if(type == Executor::MAINPROCESS){
        CellInfoWidget *cell = getCellWidget(objname);
        cell->scheduled();
    }
}

void ProcessShowTable::setProcessStopped(QString objname)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->scheduled();
}

void ProcessShowTable::setProcessError(QString objname, int currentnum)
{
    CellInfoWidget *cell = getCellWidget(objname);
    cell->updateErrorProgress(currentnum);
}

void ProcessShowTable::setProcessErrorText(QString objectname, QString str)
{
    CellInfoWidget *cell = getCellWidget(objectname);
    cell->updateErrorText(str);
}

void ProcessShowTable::launchConsole(QString objname)
{

}

CellInfoWidget* ProcessShowTable::getCellWidget(QString objname)
{
    //search object name
    int count = this->rowCount();
    if(count < 1) return nullptr;

    QWidget *wid = nullptr;
    for(int i = 0; i < count; i++){
        wid = this->cellWidget(i,0);
        if(wid->objectName() == objname) break;
    }

    CellInfoWidget *widget;
    if(wid == nullptr){
        widget = nullptr;
    }else{
        widget = qobject_cast<CellInfoWidget *>(wid);
    }

    return widget;
}

void ProcessShowTable::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    connect(taskc, &TaskSchedulerConnector::processInitCount, this, &ProcessShowTable::setProgressRange);
    connect(taskc, &TaskSchedulerConnector::processCurrent, this, &ProcessShowTable::updateProgressBar);
    connect(taskc, &TaskSchedulerConnector::processMessage, this, &ProcessShowTable::setUpdateProcess);
    connect(taskc, &TaskSchedulerConnector::timerStarted, this, &ProcessShowTable::timerStarted);
    connect(taskc, &TaskSchedulerConnector::encounteredScheduledTime, this, &ProcessShowTable::encounterdTime);
    connect(taskc, &TaskSchedulerConnector::processEnd, this, &ProcessShowTable::setProcessEnd);
    connect(taskc, &TaskSchedulerConnector::processStopped, this, &ProcessShowTable::setProcessStopped);
    connect(taskc, &TaskSchedulerConnector::taskEnabled, this, &ProcessShowTable::addItem);
    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &ProcessShowTable::removeItem);

    connect(this, &ProcessShowTable::pause, taskc, &TaskSchedulerConnector::processPause);
    connect(this, &ProcessShowTable::stop, taskc, &TaskSchedulerConnector::processStop);
}
