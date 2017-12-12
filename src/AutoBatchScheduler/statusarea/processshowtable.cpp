#include "processshowtable.h"
#include "statuswidget.h"

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

    //set new xml builder
    builder = new StartupXmlBuilder();
}

ProcessShowTable::~ProcessShowTable()
{
    delete builder;
}

//void ProcessShowTable::addItem(QString objname)
//{
//    int count = this->rowCount();
//    insertRow(count);
//    CellInfoWidget *widget = new CellInfoWidget();
//    widget->setObjectName(objname);

//    //clicked operation
//    connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
//    connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
//    connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

//    //update operation
////    connect(this, &ProcessShowTable::updateProcess, widget, &CellInfoWidget::updateProcess);
////    connect(this, &ProcessShowTable::updateProgress, widget, &CellInfoWidget::updateProgress);

//    //set action
////    connect(this, &ProcessShowTable::setProgress, widget, &CellInfoWidget::setProgressminmax);

//    setRowHeight(count, widget->height());
//    setCellWidget(count, 0, widget);
//}

//void ProcessShowTable::removeItem(QString objectname)
//{
//    //search object name
//    int count = this->rowCount();
//    int row = -1;
//    QWidget *widget = nullptr;
//    for(int i = 0; i < count; i++){
//        widget = this->cellWidget(i,0);
//        if(widget->objectName() == objectname){
//            row = i;
//            break;
//        }
//    }

//    //remove pattern matched item
//    if(widget != nullptr && row > -1){
//        CellInfoWidget *ciw = qobject_cast<CellInfoWidget *>(widget);

//        //clicked operation
//        disconnect(ciw, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
//        disconnect(ciw, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
//        disconnect(ciw, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

//        //update operation
////        disconnect(this, &ProcessShowTable::updateProcess, ciw, &CellInfoWidget::updateProcess);
////        disconnect(this, &ProcessShowTable::updateProgress, ciw, &CellInfoWidget::updateProgress);

//        //set action
////        disconnect(this, &ProcessShowTable::setProgress, ciw, &CellInfoWidget::setProgressminmax);

//        this->takeItem(row, 0);
//        this->removeCellWidget(row, 0);
//        this->removeRow(row);
//        setRowCount(count - 1);
//    }
//}

void ProcessShowTable::insertItem(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();
    insertRow(itemid);

    if(builder->readItem(itemid, list)){

        CellInfoWidget *widget = new CellInfoWidget();
        widget->setObjectName(list->at(StartupXmlBuilder::UNIQUE).at(1));
        widget->setConsoleTarget(taskc);

        //clicked operation
        connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
        connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
        connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

        //set operation
        widget->setProfileName(list->at(StartupXmlBuilder::NAME).at(1));

        bool isvalid = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? true : false;

        //set checked action
        widget->setRunStatus(isvalid);

        //insert tables

        setRowHeight(itemid, widget->indicateHeight());
        setCellWidget(itemid, 0, widget);

        //start scheduler if checkbox is valid
        if(isvalid){
            QFileInfo info(list->at(StartupXmlBuilder::PROF).at(1));
            if(info.exists()){
                taskc->enableTask(list->at(StartupXmlBuilder::UNIQUE).at(1), info.canonicalFilePath());

            }else{
                //change xml data (warning : determined order)
                changeXmlValidState(itemid);
                //show message
                emit infoNofile(list->at(StartupXmlBuilder::NAME).at(1));
            }
        }

        //clicked operation after
        connect(widget, &CellInfoWidget::changeRunStatus, this, &ProcessShowTable::onCheckStateChanged);

    }

    delete list;
}

void ProcessShowTable::enableItem(QString objname)
{
    //TODO: case of no cellwidget (when Item add newly)
    CellInfoWidget *ciw = getCellWidget(objname);

    if(ciw != nullptr){
        ciw->setRunStatus(true);
    }
}

void ProcessShowTable::disableItem(QString objname)
{
    CellInfoWidget *ciw = getCellWidget(objname);

    if(ciw != nullptr){
        ciw->setRunStatus(false);
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
//    ConsoleWidget *cwid = new ConsoleWidget();

////    QRect rec = qobject_cast<StatusWidget *>(this->parent())->rect();
////    int d_width = rec.topRight().rx();

////    cwid->move(d_width - cwid->width() - 30, 70);
//    cwid->setWindowTitle(tr("console"));
//    cwid->setConsoleViewTarget(taskc, objname);
//    cwid->show();

    //search object name
    int count = this->rowCount();
    int tableid = -1;
    if(count < 1) return;

    QWidget *wid = nullptr;
    for(int i = 0; i < count; i++){
        wid = this->cellWidget(i,0);
        if(wid->objectName() == objname){
            tableid = i;
            break;
        }
    }

    CellInfoWidget *widget = (wid == nullptr)? nullptr : qobject_cast<CellInfoWidget *>(wid);


    if(widget->isConsoleVisible()){
        //hide console
        widget->setConsoleVisible(false);
        qDebug() << "Row Height: " << widget->indicateHeight();
        setRowHeight(tableid, widget->indicateHeight());
    }else{
        //show console
        widget->setConsoleVisible(true);

        qDebug() << "Row Height: " << widget->baseSize().height();

        setRowHeight(tableid, widget->baseSize().height());
    }
}

void ProcessShowTable::onCheckStateChanged(bool checked)
{
    QString objname = this->sender()->objectName();
    int itemid = getStartupXmlIndex(objname);

    if(itemid < 0) return;

    if(checked){
        QList<QStringList> list;
        if(builder->readItem(itemid, &list)){
            QFileInfo info(list.at(StartupXmlBuilder::PROF).at(1));
            if(info.exists()){
                //change xml data (warning : determined order)
                changeXmlValidState(itemid);
                taskc->enableTask(objname, info.canonicalFilePath());

            }else{
                //show message
                emit infoNofile(list.at(StartupXmlBuilder::NAME).at(1));
            }
        }
    }else{
        taskc->disableTask(objname);
        //change xml data
        changeXmlValidState(itemid);
    }
}

void ProcessShowTable::initCellWidgets()
{
    int count = builder->count();

    for(int i = 0; i < count; i++){
        insertItem(i);
    }
}

void ProcessShowTable::changeXmlValidState(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    if(builder->readItem(itemid, list)){

        QString identifier = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? "no" : "yes";

        //change validation
        QStringList tmp;
        tmp << "valid" << identifier;
        list->removeAt(StartupXmlBuilder::VALID);
        list->insert(StartupXmlBuilder::VALID, tmp);

        builder->editItem(itemid, list);
    }

    delete list;
}

int ProcessShowTable::getStartupXmlIndex(QString objectname)
{
    QList<QStringList> *list = new QList<QStringList>();

    //search valid data
    int itemid = -1;
    int count = builder->count();

    for(int i = 0; i < count; i++){
        list->clear();
        if(builder->readItem(i, list)
                && objectname == list->at(StartupXmlBuilder::UNIQUE).at(1)){
            itemid = i;
            break;
        }
    }

    delete list;
    return itemid;
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

    CellInfoWidget *widget = (wid == nullptr)? nullptr : qobject_cast<CellInfoWidget *>(wid);

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
    connect(taskc, &TaskSchedulerConnector::taskEnabled, this, &ProcessShowTable::enableItem);
    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &ProcessShowTable::disableItem);

    connect(this, &ProcessShowTable::pause, taskc, &TaskSchedulerConnector::processPause);
    connect(this, &ProcessShowTable::stop, taskc, &TaskSchedulerConnector::processStop);

    initCellWidgets();
}
