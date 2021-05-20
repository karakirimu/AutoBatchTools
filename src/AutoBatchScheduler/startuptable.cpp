/*
 * Copyright 2016-2021 karakirimu
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
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name") << tr("Profile") << tr("Status")));

    //set header style
    QssPropertyConstant qpc;
    verticalHeader()->setProperty(qpc.VERTICAL_HEADER_STYLE \
                                    , qpc.VERTICAL_HEADER_ENABLE);

    //set doubleclick action
    connect(this, &QTableWidget::cellDoubleClicked, this, &StartupTable::editTableAction);
}

StartupTable::~StartupTable()
{
}

void StartupTable::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    connect(taskc, &TaskSchedulerConnector::fileLoadCompleted, this, &StartupTable::reloadAction);
    connect(taskc, &TaskSchedulerConnector::updateState, this, &StartupTable::stateChanged);
}

void StartupTable::setPopupActionTop()
{
    //set basic items
    m_add = addTableAction(ACTION::ADD, Qt::CTRL, Qt::Key_Enter);
    m_delete = addTableAction(ACTION::REMOVE, Qt::Key_Delete);
    contextMenu->addSeparator();

    m_edit = addTableAction(ACTION::EDIT, Qt::CTRL, Qt::Key_E);
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

    m_copy = addTableAction(ACTION::COPY, Qt::CTRL, Qt::Key_C);

    contextMenu->addSeparator();
    connect(m_copy, &QAction::triggered, this, &StartupTable::copyAction);
    connect(m_enable, &QAction::triggered, this, &StartupTable::enableAction);
    connect(m_disable, &QAction::triggered, this, &StartupTable::disableAction);
}

void StartupTable::setPopupActionBottom()
{
    m_up = addTableAction(ACTION::UP, Qt::CTRL, Qt::Key_Up);
    m_down = addTableAction(ACTION::DOWN, Qt::CTRL, Qt::Key_Down);

    contextMenu->addSeparator();
    m_ref = addTableAction(ACTION::REFRESH, Qt::CTRL, Qt::Key_R);

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

int StartupTable::getIndex(QString objectname)
{
    //search valid data
    int itemid = -1;
    QList<SchedulerCache> list = taskc->readAll();

    for(const SchedulerCache& sc : list){
        itemid++;
        if(objectname == sc.objectName()) break;
    }

    return itemid;
}

void StartupTable::setTableItem(int row)
{
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
    QString state = sc.isScheduled ? tr("Enabled") : tr("Disabled");
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
    sd->move(this->window()->mapToGlobal(this->geometry().center()) - sd->rect().center());

    if(sd->exec() == QDialog::Accepted){
        taskc->add(sd->getSavedSetting());

        int index = this->rowCount();
        setRowCount(index + 1);
        setTableItem(index);
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

    sd->load(taskc->read(row));

    if(sd->exec() == QDialog::Accepted) {
        SchedulerCache result = sd->getSavedSetting();
        taskc->replace(row, result);
        setTableItem(row);
    }

    delete sd;
}

void StartupTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage()) {
        taskc->remove(currentRow());
        reloadAction();
    }
}

void StartupTable::reloadAction()
{
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

    taskc->duplicate(this->currentRow());

    setRowCount(count + 1);
    setTableItem(count);
}

void StartupTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    taskc->move(current, current - 1);

    reloadAction();
    selectRow(current - 1);
}

void StartupTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    taskc->move(current, current + 1);

    reloadAction();
    selectRow(current + 1);
}

void StartupTable::enableAction(){
    int row = this->currentRow();

    taskc->enableSchedule(row);
    replaceItem(row);
}

void StartupTable::disableAction(){
    int row = this->currentRow();

    taskc->disableSchedule(row);
    replaceItem(row);
}
