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
//    builder = new StartupXmlBuilder();
}

ProcessShowTable::~ProcessShowTable()
{
//    delete builder;
}

void ProcessShowTable::removeItem(int itemid)
{
    //delete table only
    this->removeRow(itemid);
}

void ProcessShowTable::insertItem(int itemid)
{
//    QList<QStringList> *list = new QList<QStringList>();
//    insertRow(itemid);

//    if(builder->readItem(itemid, list)){

//        CellInfoWidget *widget = new CellInfoWidget();
//        widget->setObjectName(list->at(StartupXmlBuilder::UNIQUE).at(1));
//        widget->setConsoleTarget(taskc);

//        //clicked operation
//        connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
//        connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
//        connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

//        //set operation
//        widget->setProfileName(list->at(StartupXmlBuilder::NAME).at(1));

//        bool isvalid = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? true : false;

//        //set checked action
//        widget->setRunStatus(isvalid);

//        //insert tables

//        setRowHeight(itemid, widget->indicateHeight());
//        setCellWidget(itemid, 0, widget);

//        //clicked operation after
//        connect(widget, &CellInfoWidget::changeRunStatus, this, &ProcessShowTable::onCheckStateChanged);

//    }

//    delete list;

    SchedulerCache cache = taskc->read(itemid);
    insertRow(itemid);

    CellInfoWidget *widget = new CellInfoWidget();
    widget->setObjectName(cache.objectName());
    widget->setConsoleTarget(taskc);

    //clicked operation
    connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
    connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
    connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

    //set operation
    widget->setProfileName(cache.settingName);

    //set checked action
    widget->setRunStatus(cache.isScheduled);

    //insert tables
    setRowHeight(itemid, widget->indicateHeight());
    setCellWidget(itemid, 0, widget);

    //clicked operation after
    connect(widget, &CellInfoWidget::changeRunStatus, this, &ProcessShowTable::onCheckStateChanged);

}

void ProcessShowTable::replaceItem(int itemid)
{
    this->removeRow(itemid);
    this->insertItem(itemid);
}

void ProcessShowTable::moveItem(int before, int after)
{
    int count = this->rowCount();
    if(count <= before || count <= after ) return;

    CellInfoWidget *fWidget = qobject_cast<CellInfoWidget *>(cellWidget(before, 0));
    QHash<int, QVariant> state = fWidget->getState();

    removeItem(before);
    insertItem(after);

    CellInfoWidget *tWidget = qobject_cast<CellInfoWidget *>(cellWidget(after, 0));
    tWidget->restoreState(state);
}

void ProcessShowTable::duplicateItem()
{
    int count = this->rowCount();
    this->insertItem(count);
}

void ProcessShowTable::enableItem(QString objname)
{
    //TODO: case of no cellwidget (when Item add newly)
    CellInfoWidget *ciw = getCellWidget(objname);

    if(ciw != nullptr){
        ciw->setRunStatus(true);
    }
}

void ProcessShowTable::enableItem(int index)
{
    if(index >= rowCount()) return;
    CellInfoWidget *ciw = qobject_cast<CellInfoWidget *>(this->cellWidget(index,0));
    ciw->setRunStatus(true);
}

void ProcessShowTable::disableItem(QString objname)
{
    CellInfoWidget *ciw = getCellWidget(objname);

    if(ciw != nullptr){
        ciw->setRunStatus(false);
    }
}

void ProcessShowTable::disableItem(int index)
{
    if(index >= rowCount()) return;
    CellInfoWidget *ciw = qobject_cast<CellInfoWidget *>(this->cellWidget(index,0));
    ciw->setRunStatus(false);
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
    qDebug() << "[ProcessShowTable::timerStarted] Timer started.";
    CellInfoWidget *cell = getCellWidget(objname);
    cell->setTimerEnd(date.toString(tr("yyyy/MM/dd HH:mm:ss")));
}

void ProcessShowTable::timerStopped(QString objectname, int type)
{
    qDebug() << "[ProcessShowTable::timerStopped] Timer stopped.";

    CellInfoWidget *cell = getCellWidget(objectname);

    switch (type) {
    case SchedulerWait::FINISHED:
        cell->updateProcess(tr(""), -1);
        break;
    case SchedulerWait::EXPIRED:
        cell->updateProcess(tr(" It is already passed the starting time."), -1);
        break;
    }
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
//    int itemid = getStartupXmlIndex(objname);
    int itemid = getIndex(objname);

    if(itemid < 0) return;

    if(checked){
//        QList<QStringList> list;
//        if(builder->readItem(itemid, &list)){
//            QFileInfo info(list.at(StartupXmlBuilder::PROF).at(1));
//            if(info.exists()){
//                //change xml data (warning : determined order)

//                changeXmlValidState(itemid);
//                taskc->enableTask(objname, info.canonicalFilePath());

//            }else{
//                //show message
//                emit infoNofile(list.at(StartupXmlBuilder::NAME).at(1));
//            }
//        }
        taskc->enableSchedule(itemid);
    }else{
        //change xml data
//        changeXmlValidState(itemid);
//        taskc->disableTask(objname);
        taskc->disableSchedule(itemid);
    }
}

void ProcessShowTable::initCellWidgets()
{
//    int count = builder->count();

//    for(int i = 0; i < count; i++){
//        initCellWidget(i);
//    }
    QList<SchedulerCache> list = taskc->readAll();
    int count = list.count();

    for(int i = 0; i < count; i++){
        initCellWidget(i, list.at(i));
    }

}

void ProcessShowTable::initCellWidget(int itemid)
{
//    QList<QStringList> *list = new QList<QStringList>();
//    insertRow(itemid);

//    if(builder->readItem(itemid, list)){

//        CellInfoWidget *widget = new CellInfoWidget();
//        widget->setObjectName(list->at(StartupXmlBuilder::UNIQUE).at(1));
//        widget->setConsoleTarget(taskc);

//        //clicked operation
//        connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
//        connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
//        connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

//        //set operation
//        widget->setProfileName(list->at(StartupXmlBuilder::NAME).at(1));

//        bool isvalid = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? true : false;

//        //set checked action
//        widget->setRunStatus(isvalid);

//        //insert tables
//        setRowHeight(itemid, widget->indicateHeight());
//        setCellWidget(itemid, 0, widget);

//        //start scheduler if checkbox is valid
//        if(isvalid){
//            QFileInfo info(list->at(StartupXmlBuilder::PROF).at(1));
//            if(info.exists()){
//                taskc->enableTask(list->at(StartupXmlBuilder::UNIQUE).at(1), info.canonicalFilePath());

//            }else{
//                //change xml data (warning : determined order)
//                changeXmlValidState(itemid);
//                //show message
//                emit infoNofile(list->at(StartupXmlBuilder::NAME).at(1));
//            }
//        }

//        //clicked operation after
//        connect(widget, &CellInfoWidget::changeRunStatus, this, &ProcessShowTable::onCheckStateChanged);

//    }

//    delete list;

    SchedulerCache cache = taskc->read(itemid);
    initCellWidget(itemid, cache);
}

void ProcessShowTable::initCellWidget(int itemid, const SchedulerCache &cache)
{
    insertRow(itemid);

    CellInfoWidget *widget = new CellInfoWidget();
    widget->setObjectName(cache.objectName());
    widget->setConsoleTarget(taskc);

    //clicked operation
    connect(widget, &CellInfoWidget::consoleButtonClicked, this, &ProcessShowTable::launchConsole);
    connect(widget, &CellInfoWidget::pauseButtonClicked, this, &ProcessShowTable::pauseClicked);
    connect(widget, &CellInfoWidget::stopButtonClicked, this, &ProcessShowTable::stopClicked);

    //set operation
    widget->setProfileName(cache.settingName);

    //set checked action
    widget->setRunStatus(cache.isScheduled);

    //insert tables
    setRowHeight(itemid, widget->indicateHeight());
    setCellWidget(itemid, 0, widget);

    //start scheduler if checkbox is valid
    if(cache.isScheduled){
        taskc->updateState(itemid, cache.objectName(), TaskSchedulerConnector::ENABLE);
    }

    //clicked operation after
    connect(widget, &CellInfoWidget::changeRunStatus, this, &ProcessShowTable::onCheckStateChanged);
}

int ProcessShowTable::getIndex(QString objectname)
{
    QList<SchedulerCache> list = taskc->readAll();

    int count = list.count();

    for(int i = 0; i < count; i++){
        if(objectname == list.at(i).objectName()){
            return i;
        }
    }

    return -1;
}

//void ProcessShowTable::changeXmlValidState(int itemid)
//{
//    QList<QStringList> *list = new QList<QStringList>();

//    if(builder->readItem(itemid, list)){

//        QString identifier = (list->at(StartupXmlBuilder::VALID).at(1) == "yes")? "no" : "yes";

//        //change validation
//        list->removeAt(StartupXmlBuilder::VALID);
//        list->insert(StartupXmlBuilder::VALID, QStringList() << "valid" << identifier);

//        builder->editItem(itemid, list);
//    }

//    delete list;
//}

//int ProcessShowTable::getStartupXmlIndex(QString objectname)
//{
//    QList<QStringList> *list = new QList<QStringList>();

//    //search valid data
//    int itemid = -1;
//    int count = builder->count();

//    for(int i = 0; i < count; i++){
//        list->clear();
//        if(builder->readItem(i, list)
//                && objectname == list->at(StartupXmlBuilder::UNIQUE).at(1)){
//            itemid = i;
//            break;
//        }
//    }

//    delete list;
//    return itemid;
//}

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
    connect(taskc, &TaskSchedulerConnector::timerFinished, this, &ProcessShowTable::timerStopped);
    connect(taskc, &TaskSchedulerConnector::encounteredScheduledTime, this, &ProcessShowTable::encounterdTime);
    connect(taskc, &TaskSchedulerConnector::processEnd, this, &ProcessShowTable::setProcessEnd);
    connect(taskc, &TaskSchedulerConnector::processStopped, this, &ProcessShowTable::setProcessStopped);
//    connect(taskc, &TaskSchedulerConnector::taskEnabled, this, &ProcessShowTable::enableItem);
//    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &ProcessShowTable::disableItem);

//    connect(taskc, &TaskSchedulerConnector::tableMessenger, this, &ProcessShowTable::tableChanged);
    connect(taskc, &TaskSchedulerConnector::updateState, this, &ProcessShowTable::stateChanged);

    connect(this, &ProcessShowTable::pause, taskc, &TaskSchedulerConnector::processPause);
    connect(this, &ProcessShowTable::stop, taskc, &TaskSchedulerConnector::processStop);


    connect(taskc, &TaskSchedulerConnector::fileLoadCompleted
            , this, &ProcessShowTable::initCellWidgets);
}

//void ProcessShowTable::tableChanged(QString message, TaskSchedulerConnector::TABLE func)
//{
//    switch (func) {
//    case TaskSchedulerConnector::TABLE::ADD:
//    case TaskSchedulerConnector::TABLE::INSERT:   insertItem(message.toInt());   break;
//    case TaskSchedulerConnector::TABLE::DELETE:   removeItem(message.toInt());   break;
//    case TaskSchedulerConnector::TABLE::EDIT:     replaceItem(message.toInt());  break;
//    case TaskSchedulerConnector::TABLE::ENABLE:   enableItem(message);           break;
//    case TaskSchedulerConnector::TABLE::DISABLE:  disableItem(message);          break;
//    case TaskSchedulerConnector::TABLE::DUPLICATE: break;
//    case TaskSchedulerConnector::TABLE::DRAGDROP: break;
//    case TaskSchedulerConnector::TABLE::UP: break;
//    case TaskSchedulerConnector::TABLE::DOWN: break;
//    case TaskSchedulerConnector::TABLE::SWAP: break;
//    case TaskSchedulerConnector::TABLE::MOVE: break;
//    }
//}

void ProcessShowTable::stateChanged(int index, QString message, TaskSchedulerConnector::TABLE func)
{
    qDebug() << "[ProcessShowTable::stateChanged] index : " << index << " action : " << func;
    Q_UNUSED(message)

    switch (func) {
    case TaskSchedulerConnector::TABLE::ADD:
    case TaskSchedulerConnector::TABLE::INSERT:   insertItem(index);  break;
    case TaskSchedulerConnector::TABLE::DELETE:   removeItem(index);  break;
    case TaskSchedulerConnector::TABLE::EDIT:     replaceItem(index); break;
    case TaskSchedulerConnector::TABLE::ENABLE:   enableItem(index);  break;
    case TaskSchedulerConnector::TABLE::DISABLE:  disableItem(index); break;
    case TaskSchedulerConnector::TABLE::DUPLICATE: duplicateItem(); break;
    case TaskSchedulerConnector::TABLE::DRAGDROP: break;
    case TaskSchedulerConnector::TABLE::UP:   break;
    case TaskSchedulerConnector::TABLE::DOWN: break;
    case TaskSchedulerConnector::TABLE::SWAP: break;
    case TaskSchedulerConnector::TABLE::MOVE: moveItem(index, message.toInt()); break;
    }
}
