/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

void ProcessShowTable::removeItem(int itemid)
{
    //delete table only
    this->removeRow(itemid);
}

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

        //clicked operation after
        connect(widget, &CellInfoWidget::changeRunStatus, this, &ProcessShowTable::onCheckStateChanged);

    }

    delete list;
}

void ProcessShowTable::replaceItem(int itemid)
{
    this->removeRow(itemid);
    this->insertItem(itemid);
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
    if(type == Executor::SCHEDULER){
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
        //change xml data
        changeXmlValidState(itemid);
        taskc->disableTask(objname);
    }
}

void ProcessShowTable::initCellWidgets()
{
    int count = builder->count();

    for(int i = 0; i < count; i++){
        initCellWidget(i);
    }
}

void ProcessShowTable::initCellWidget(int itemid)
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

void ProcessShowTable::changeXmlValidState(int itemid)
{
    QList<QStringList> *list = new QList<QStringList>();

    if(builder->readItem(itemid, list)){

        QString identifier = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? "no" : "yes";

        //change validation
        list->removeAt(StartupXmlBuilder::VALID);
        list->insert(StartupXmlBuilder::VALID, QStringList() << "valid" << identifier);

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

//    connect(taskc, &TaskSchedulerConnector::tableInserted, this, &ProcessShowTable::insertItem);
//    connect(taskc, &TaskSchedulerConnector::tableDeleted, this, &ProcessShowTable::removeItem);
//    connect(taskc, &TaskSchedulerConnector::tableReplaced, this, &ProcessShowTable::replaceItem);
//    connect(taskc, &TaskSchedulerConnector::tableEnabled, this, &ProcessShowTable::enableItem);
//    connect(taskc, &TaskSchedulerConnector::tableDisabled, this, &ProcessShowTable::disableItem);
    connect(taskc, &TaskSchedulerConnector::tableMessenger, this, &ProcessShowTable::tableChanged);

    connect(this, &ProcessShowTable::pause, taskc, &TaskSchedulerConnector::processPause);
    connect(this, &ProcessShowTable::stop, taskc, &TaskSchedulerConnector::processStop);

    initCellWidgets();
}

void ProcessShowTable::tableChanged(QString message, TaskSchedulerConnector::TABLE func)
{
    switch (func) {
    case TaskSchedulerConnector::TABLE::ADD:
    case TaskSchedulerConnector::TABLE::INSERT:   insertItem(message.toInt());   break;
    case TaskSchedulerConnector::TABLE::DELETE:   removeItem(message.toInt());   break;
    case TaskSchedulerConnector::TABLE::EDIT:     replaceItem(message.toInt());  break;
    case TaskSchedulerConnector::TABLE::ENABLE:   enableItem(message);           break;
    case TaskSchedulerConnector::TABLE::DISABLE:  disableItem(message);          break;
    case TaskSchedulerConnector::TABLE::COPY: break;
    case TaskSchedulerConnector::TABLE::DRAGDROP: break;
    case TaskSchedulerConnector::TABLE::UP: break;
    case TaskSchedulerConnector::TABLE::DOWN: break;
    case TaskSchedulerConnector::TABLE::SWAP: break;
    }
}


//bool ProcessShowTable::eventFilter(QObject *obj, QEvent *event)
//{
//    if(event->type() == QEvent::Leave){
//        this->horizontalScrollBar()->hide();
//        this->verticalScrollBar()->hide();
//    }

//    if(event->type() == QEvent::ToolTip){
//        if(this->mapFromGlobal(QCursor::pos()).y() > (this->height() - RANGE)){
//            this->horizontalScrollBar()->show();
//        }
//        if(this->mapFromGlobal(QCursor::pos()).x() > (this->width() - RANGE)){
//            this->verticalScrollBar()->show();
//        }
//    }

//    // standard event processing
//    return QObject::eventFilter(obj, event);
//}
