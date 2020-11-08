﻿/*
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

#include "startuptable.h"

StartupTable::StartupTable(QWidget *parent)
    : BasicTable(parent)
{
    //disable edit
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    //popupAction
    setPopupActionTop();
    setPopupActionDefault();
    setPopupActionBottom();

    //init table size
    setColumnCount(3);
    setRowCount(0);

    //adjust row
    resizeRowsToContents();

    //adjust column
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //set header label
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name") << tr("Profile") << tr("State")));

    //set header style
    verticalHeader()->setProperty("VerticalHeaderStyle", 1);

//    builder = new StartupXmlBuilder();

    //set doubleclick action
    connect(this, &QTableWidget::cellDoubleClicked, this, &StartupTable::editTableAction);

    //init table (reload read file.)
//    reloadAction();

}

StartupTable::~StartupTable()
{
//    delete builder;
}

void StartupTable::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    connect(taskc, &TaskSchedulerConnector::fileLoadCompleted, this, &StartupTable::reloadAction);
    connect(taskc, &TaskSchedulerConnector::updateState, this, &StartupTable::stateChanged);

//    connect(taskc, &TaskSchedulerConnector::taskEnabled, this, &StartupTable::updateItemEnabled);
//    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &StartupTable::updateItemEnabled);

    //connect
//    connect(taskc, &TaskSchedulerConnector::timerStarted, this, &SystemTray::showTimerStart);
//    connect(taskc, &TaskSchedulerConnector::timerFinished, this, &StartupTable::updateItemTimerEnd);
//    connect(taskc, &TaskSchedulerConnector::processStarted, this, &SystemTray::showProcessStart);
//    connect(taskc, &TaskSchedulerConnector::processStopped, this, &SystemTray::showProcessStopped);
//    connect(taskc, &TaskSchedulerConnector::processPaused, this, &SystemTray::showProcessPause);
//    connect(taskc, &TaskSchedulerConnector::processEnd, this, &SystemTray::showProcessEnded);
}

void StartupTable::setPopupActionTop()
{
    //set basic items
    m_add = addTableAction(ACTION::ADD, Qt::CTRL + Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL + Qt::Key_E);
    contextMenu->addSeparator();

    //connect signals
    connect(m_add, &QAction::triggered, this, &StartupTable::addAction);
    connect(m_edit, &QAction::triggered, this, &StartupTable::editAction);
    connect(m_delete, &QAction::triggered, this, &StartupTable::deleteAction);
}

void StartupTable::setPopupActionDefault()
{
    m_enable = addTableAction(ACTION::ENABLE);
    m_disable = addTableAction(ACTION::DISABLE);
    contextMenu->addSeparator();

    m_copy = addTableAction(ACTION::COPY, Qt::CTRL + Qt::Key_C);

    contextMenu->addSeparator();
    connect(m_copy, &QAction::triggered, this, &StartupTable::copyAction);
    connect(m_enable, &QAction::triggered, this, &StartupTable::enableAction);
    connect(m_disable, &QAction::triggered, this, &StartupTable::disableAction);
}

void StartupTable::setPopupActionBottom()
{
    m_up = addTableAction(ACTION::UP, Qt::CTRL + Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL + Qt::Key_Down);

    contextMenu->addSeparator();
    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL + Qt::Key_R);

    connect(m_up, &QAction::triggered, this, &StartupTable::upAction);
    connect(m_down, &QAction::triggered, this, &StartupTable::downAction);
    connect(m_ref, &QAction::triggered, this, &StartupTable::reloadAction);
}

bool StartupTable::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

    auto mdCheck = [&keyEvent](){
        return static_cast<bool>(keyEvent->modifiers() & Qt::ControlModifier);
    };

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
         {
           case Qt::Key_Return:
           case Qt::Key_Enter:       if(mdCheck()) addAction(); break;
           case Qt::Key_Delete:      deleteAction();            break;

           case Qt::Key_Up:
             if(mdCheck()){
                 upAction();
             }else{
                 if(this->currentRow() != 0)
                     selectRow(this->currentRow() - 1);
             }
             break;

           case Qt::Key_Down:
             if(mdCheck()){
                 downAction();
             }else{
                 if(this->rowCount() - 1 != this->currentRow())
                     selectRow(this->currentRow() + 1);
             }
            break;

           case Qt::Key_C:          if(mdCheck()) copyAction();   break;
           case Qt::Key_E:          if(mdCheck()) editAction();   break;
           case Qt::Key_R:          if(mdCheck()) reloadAction(); break;

           default:
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

int StartupTable::getStartupXmlIndex(QString objectname)
{
//    copy of systemtray
//    QList<QStringList> *list = new QList<QStringList>();

    //search valid data
    int itemid = -1;
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
    QList<SchedulerCache> list = taskc->readAll();

    for(SchedulerCache sc : list){
        itemid++;
        if(objectname == sc.objectName()) break;
    }

    return itemid;
}

void StartupTable::setTableItem(int row)
{
    //qDebug () << "setTableItem";
//    QList<QStringList> *list = new QList<QStringList>();
//    if(builder->readItem(row, list)){
//        //set name
//        this->setItem(row,0,new QTableWidgetItem(list->at(StartupXmlBuilder::NAME).at(1)));

//        //set profile
//        QFileInfo info(list->at(StartupXmlBuilder::PROF).at(1));
//        this->setItem(row,1,new QTableWidgetItem(info.fileName()));

//        //set icon
//        QIcon icon;
//        if(VariantConverter::stringToBool(list->at(StartupXmlBuilder::VALID).at(1))){
//            icon = getIcon(ACTION::ENABLE);
//        }else{
//            icon = getIcon(ACTION::DISABLE);
//        }

//        this->setItem(row,2,new QTableWidgetItem(QIcon(icon),list->at(2).at(1)));
//    }

//    delete list;

    SchedulerCache sc = taskc->read(row);
    setTableItem(row, sc);

}

void StartupTable::setTableItem(int row, const SchedulerCache &sc)
{
    //set name
    this->setItem(row,0,new QTableWidgetItem(sc.settingName));

    //set profile
    QFileInfo info(sc.profilePath);
    this->setItem(row,1,new QTableWidgetItem(info.fileName()));

    //set icon
    QIcon icon = getIcon(sc.isScheduled ? ACTION::ENABLE : ACTION::DISABLE);
    QString state = VariantConverter::boolToString(sc.isScheduled);
    this->setItem(row,2,new QTableWidgetItem(QIcon(icon), state));
}

void StartupTable::replaceItem(int row)
{
    this->removeRow(row);
    this->insertRow(row);
    setTableItem(row);
    this->selectRow(row);
}

void StartupTable::addAction()
{
    StartupDialog *sd = new StartupDialog();
    sd->setStyleSheet(this->styleSheet());
//    sd->setWindowTitle(tr("Editing - Untitled*"));
    sd->move(this->window()->mapToGlobal(this->geometry().center()) - sd->rect().center());

    if(sd->exec() == QDialog::Accepted){
        taskc->add(sd->getSavedSetting());

        int index = this->rowCount();
        setRowCount(index + 1);
        setTableItem(index);

//        emit taskc->tableMessenger(index,\
//                                   TaskSchedulerConnector::TABLE::ADD);
    }

    delete sd;
}

void StartupTable::editAction()
{
    editTableAction(currentRow(), 0);
}

void StartupTable::editTableAction(int row, int col)
{
    Q_UNUSED(col)
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    StartupDialog *sd = new StartupDialog();
    sd->setStyleSheet(this->styleSheet());
    sd->move(this->window()->mapToGlobal(this->geometry().center()) - sd->rect().center());

//    QList<QStringList> list;
//    int rows = row;

//    if(builder->readItem(rows, &list)){
//        set title
//        sd->loadSettingList(rows, &list);
//    }
    sd->load(taskc->read(row));

    if(sd->exec() == QDialog::Accepted){
        SchedulerCache result = sd->getSavedSetting();
        taskc->replace(row, result);
        setTableItem(row);
    }

//    emit taskc->tableMessenger(row,\
//                               TaskSchedulerConnector::TABLE::EDIT);
    delete sd;
}

void StartupTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
    {
//        QList<QStringList> list;
//        if(builder->readItem(currentRow(), &list)
//                && VariantConverter::stringToBool(list.at(StartupXmlBuilder::VALID).at(1))){
//            taskc->disableTask(list.at(StartupXmlBuilder::UNIQUE).at(1));
//        }

//        //delete file item
//        builder->deleteItem(currentRow());

//        emit taskc->tableMessenger(currentRow(),\
//                                   TaskSchedulerConnector::TABLE::DELETE);

//        taskc->updateCache(currentRow(), TaskSchedulerConnector::DISABLE);
//        taskc->updateCache(currentRow(), TaskSchedulerConnector::DELETE);
        taskc->remove(currentRow());

        //reload
        reloadAction();
    }
}

void StartupTable::reloadAction()
{
//    int count = builder->count();
//    //set tables
//    setRowCount(count);
//    for(int i = 0; i < count; i++){
//        setTableItem(i);
//    }

    QList<SchedulerCache> list = taskc->readAll();

    int count = list.count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i, list.at(i));
    }
}

void StartupTable::stateChanged(int index, QString message, TaskSchedulerConnector::TABLE func)
{
    qDebug() << "[StartupTable::stateChanged] index : " << index << " action : " << func;
    Q_UNUSED(message)

    switch (func) {
    case TaskSchedulerConnector::TABLE::ADD:
    case TaskSchedulerConnector::TABLE::INSERT: break;
    case TaskSchedulerConnector::TABLE::DELETE: break;
    case TaskSchedulerConnector::TABLE::EDIT:   break;
    case TaskSchedulerConnector::TABLE::ENABLE:
    case TaskSchedulerConnector::TABLE::DISABLE: replaceItem(index); break;
    case TaskSchedulerConnector::TABLE::DUPLICATE: break;
    case TaskSchedulerConnector::TABLE::DRAGDROP: break;
    case TaskSchedulerConnector::TABLE::UP: break;
    case TaskSchedulerConnector::TABLE::DOWN: break;
    case TaskSchedulerConnector::TABLE::SWAP: break;
    case TaskSchedulerConnector::TABLE::MOVE: break;
    }
}

void StartupTable::copyAction()
{
    //if rowcount is zero.
    int count = rowCount();
    if(count == 0) return;

//    builder->copyItem(this->currentRow());

    //update unique code
//    int currentrow = this->rowCount();
//    QList<QStringList> *list = new QList<QStringList>();
//    if(builder->readItem(currentrow, list)){
//        //modify unique strings
//        list->removeAt(StartupXmlBuilder::UNIQUE);
//        list->insert(StartupXmlBuilder::UNIQUE, QStringList() << "unique" << getRandomString(32));

//        // update item
//        builder->editItem(currentrow, list);

//        emit taskc->tableMessenger(currentrow,\
//                                   TaskSchedulerConnector::TABLE::INSERT);
//    }
//    taskc->updateCache(currentrow, TaskSchedulerConnector::DUPLICATE);
    taskc->duplicate(this->currentRow());

    setRowCount(count + 1);
    setTableItem(count);

//    reloadAction();

//    selectRow(currentrow);
}

void StartupTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

//    builder->swapItem(current, current-1);
//    taskc->updateCache(current, TaskSchedulerConnector::UP);

    taskc->move(current, current - 1);

    reloadAction();

    selectRow(current - 1);
}

void StartupTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

//    builder->swapItem(current, current+1);
//    taskc->updateCache(current, TaskSchedulerConnector::DOWN);
    taskc->move(current, current + 1);

    reloadAction();

    selectRow(current + 1);
}

void StartupTable::enableAction(){

    QList<QStringList> list;
    int row = this->currentRow();

//    if(!builder->readItem(row, &list)) return;

//    if(!VariantConverter::stringToBool(list.at(StartupXmlBuilder::VALID).at(1))){
//        QFileInfo info(list.at(StartupXmlBuilder::PROF).at(1));

//        if(info.exists()){

//            //change validation
////            emit taskc->tableMessenger(list.at(StartupXmlBuilder::UNIQUE).at(1),\
////                                       TaskSchedulerConnector::TABLE::ENABLE);
//            replaceItem(row);
//        }

//    }
    taskc->enableSchedule(row);
    replaceItem(row);
}

void StartupTable::disableAction(){
    //FIXME: NO CONDITION APPLYED IN ... (maybe solved)

    QList<QStringList> list;
    int row = this->currentRow();

//    if(!builder->readItem(row, &list)) return;

//    if(VariantConverter::stringToBool(list.at(StartupXmlBuilder::VALID).at(1))){

//        //change validation
////        emit taskc->tableMessenger(list.at(StartupXmlBuilder::UNIQUE).at(1),\
////                                   TaskSchedulerConnector::TABLE::DISABLE);
//        taskc->updateCache(row, TaskSchedulerConnector::DISABLE);
//        replaceItem(row);
//    }

    taskc->disableSchedule(row);
    replaceItem(row);
}

//void StartupTable::updateItemEnabled(QString objectname)
//{
//    int itemid = getStartupXmlIndex(objectname);
//    if(itemid > -1) setTableItem(itemid);
//}

//void StartupTable::updateItemTimerEnd(QString objectname, int status)
//{
//    Q_UNUSED(status)
//    qDebug() << "[StartupTable::updateItemTimerEnd] ";
////    switch(status){
////    case SchedulerWait::FINISHED:
////        break;
////    case SchedulerWait::EXPIRED:
////        break;
////    }

//    updateItemEnabled(objectname);
//}

//QString StartupTable::getRandomString(int length)
//{
//    QString characters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.");

//    //shuffle characters
//    int pos = characters.size() - 1;
//    int random;
//    while(pos > 1){
//        random = abs(static_cast<int>(QRandomGenerator::global()->generate()) % pos);
//        QChar tmp = characters.at(random);
//        characters.replace(random, 1, characters.at(pos));
//        characters.replace(pos, 1, tmp);
//        pos--;
//    }

//    //select characters
//    QString randomString;
//    for(int i=0; i < length; ++i)
//    {
//        int index = abs(static_cast<int>(QRandomGenerator::global()->generate()) % length);
//        QChar nextChar = characters.at(index);
//        randomString.append(nextChar);
//    }
//    return randomString;
//}
