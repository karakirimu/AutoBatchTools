﻿#include "startuptable.h"

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
    setHorizontalHeaderLabels((QStringList() << tr("Setting Name") << tr("Profile") << tr("Run Progress")));

    builder = new StartupXmlBuilder();

    //set doubleclick action
    connect(this, &QTableWidget::cellDoubleClicked, this, &StartupTable::editTableAction);

    //init table (reload read file.)
    reloadAction();

    //set current time
    QDateTime time = QDateTime::currentDateTime();
    qsrand(static_cast<uint>(time.currentSecsSinceEpoch() ^ 165423987));
}

StartupTable::~StartupTable()
{
    delete builder;
}

void StartupTable::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
//    connect(taskc, &TaskSchedulerConnector::xmlStateChanged, this, &StartupTable::updateItemEnabled);
    connect(taskc, &TaskSchedulerConnector::taskEnabled, this, &StartupTable::updateItemEnabled);
    connect(taskc, &TaskSchedulerConnector::taskDisabled, this, &StartupTable::updateItemEnabled);
//    connect(this, &StartupTable::actionDeleted, task, &TaskSchedulerConnector::actionDeleted);
//    connect(this, &StartupTable::actionAdded, task, &TaskSchedulerConnector::actionAdded);
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

    //qDebug() << event->type();
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
             //qDebug("Ate key press %d", keyEvent->key());
             break;
         }
        return true;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

int StartupTable::getStartupXmlIndex(QString objectname)
{
    //copy of systemtray
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

////QSS_THEME
//void StartupTable::themeChangeAction(StartupDialog *sd)
//{
//    QSettings settings( "./settings.ini", QSettings::IniFormat );

//    //theme settings
//    settings.beginGroup("scheduler_startup");
//    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
//    settings.endGroup();

//    if(stylecolor != "Default"){
//#ifdef QT_DEBUG
//        QFile file(QString("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/%1.qss").arg(stylecolor));
//#else
//        QFile file(QString(":/themes/%1.qss").arg(stylecolor));
//#endif
//        if(file.open( QFile::ReadOnly | QFile::Text )){
//            QString data(QLatin1String(file.readAll()));
//            sd->setStyleSheet(data);
//        }
//    }
//}

void StartupTable::setTableItem(int row)
{
    //qDebug () << "setTableItem";
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(row, list)){
        //set tableitem
        this->setItem(row,0,new QTableWidgetItem(list->at(0).at(1)));
        this->setItem(row,1,new QTableWidgetItem(list->at(1).at(1)));

        //set icon
        QString icon;
        if(VariantConverter::stringToBool(list->at(StartupXmlBuilder::VALID).at(1))){
            icon = ":/default_icons/enable.png";
        }else{
            icon = ":/default_icons/remove.png";
        }

        this->setItem(row,2,new QTableWidgetItem(QIcon(icon),list->at(2).at(1)));
    }
    delete list;
}

void StartupTable::replaceItem(int row)
{
    qDebug() << "StartupTable :: column count start: " << this->rowCount();
    //FIXME: quick fix column
    for(int i = 0; i < 3; i++){
        this->takeItem(row, i);
    }
    qDebug() << "StartupTable :: column count proc later: " << this->rowCount();

    setTableItem(row);

    qDebug() << "StartupTable :: column count end: " << this->rowCount();
//    this->insertRow(row);
}

void StartupTable::addAction()
{
    StartupDialog *sd = new StartupDialog();
//    themeChangeAction(sd);
    sd->setStyleSheet(this->styleSheet());
    sd->setWindowTitle(tr("Editing - New_file*"));
    if(sd->exec() == QDialog::Accepted){
        int index = this->rowCount();
        setRowCount(index + 1);

        setTableItem(index);

        emit taskc->tableMessenger(QString::number(index),\
                                   TaskSchedulerConnector::TABLE::ADD);
    }
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

//    QString yesno = "";
    StartupDialog *sd = new StartupDialog();
//    themeChangeAction(sd);
    sd->setStyleSheet(this->styleSheet());

    QList<QStringList> list;
    int rows = row;

    if(builder->readItem(rows, &list)){
        //set title
        sd->loadSettingList(rows, &list);
//        yesno = list.at(StartupXmlBuilder::VALID).at(1);
    }

    if(sd->exec() == QDialog::Accepted){
        setTableItem(row);
    }

    emit taskc->tableMessenger(QString::number(row),\
                               TaskSchedulerConnector::TABLE::EDIT);
}

void StartupTable::deleteAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    //check delete warning message
    if(deleteCheckMessage())
    {
        QList<QStringList> list;
        if(builder->readItem(currentRow(), &list)
                && VariantConverter::stringToBool(list.at(StartupXmlBuilder::VALID).at(1))){
            taskc->disableTask(list.at(StartupXmlBuilder::UNIQUE).at(1));
        }

        //delete file item
        builder->deleteItem(currentRow());

        emit taskc->tableMessenger(QString::number(currentRow()),\
                                   TaskSchedulerConnector::TABLE::DELETE);

        //reload
        reloadAction();
    }
}

void StartupTable::reloadAction()
{
    int count = builder->count();
    //set tables
    setRowCount(count);
    for(int i = 0; i < count; i++){
        setTableItem(i);
    }
}

void StartupTable::copyAction()
{
    //if rowcount is zero.
    if(this->rowCount() == 0) return;

    builder->copyItem(this->currentRow());

    //update unique code
    int currentrow = this->rowCount();
    QList<QStringList> *list = new QList<QStringList>();
    if(builder->readItem(currentrow, list)){
        //modify unique strings
        list->removeAt(StartupXmlBuilder::UNIQUE);
        list->insert(StartupXmlBuilder::UNIQUE, QStringList() << "unique" << getRandomString(32));

        // update item
        builder->editItem(currentrow, list);

        emit taskc->tableMessenger(QString::number(currentrow),\
                                   TaskSchedulerConnector::TABLE::INSERT);
    }

    reloadAction();

    selectRow(currentrow);
}

void StartupTable::upAction()
{
    int current = this->currentRow();
    if(current == 0) return;

    builder->swapItem(current, current-1);

    reloadAction();

    selectRow(current - 1);
}

void StartupTable::downAction()
{
    int current = this->currentRow();
    int counter = this->rowCount();

    if((current + 1) == counter) return;

    builder->swapItem(current, current+1);

    reloadAction();

    selectRow(current + 1);
}

void StartupTable::enableAction(){

    QList<QStringList> *list = new QList<QStringList>();
    int row = this->currentRow();

    if(builder->readItem(row, list)){
        if(!VariantConverter::stringToBool(list->at(StartupXmlBuilder::VALID).at(1))){
            QFileInfo info(list->at(StartupXmlBuilder::PROF).at(1));
            if(info.exists()){

                //change validation
                emit taskc->tableMessenger(list->at(StartupXmlBuilder::UNIQUE).at(1),\
                                           TaskSchedulerConnector::TABLE::ENABLE);

                replaceItem(row);
            }
        }
    }
}

void StartupTable::disableAction(){
    //FIXME: NO CONDITION APPLYED IN ...

    QList<QStringList> *list = new QList<QStringList>();
    int row = this->currentRow();

    if(builder->readItem(row, list)){

        if(VariantConverter::stringToBool(list->at(StartupXmlBuilder::VALID).at(1))){

            //change validation
            emit taskc->tableMessenger(list->at(StartupXmlBuilder::UNIQUE).at(1),\
                                       TaskSchedulerConnector::TABLE::DISABLE);
            replaceItem(row);
        }
    }
}

void StartupTable::updateItemEnabled(QString objectname)
{
    int itemid = getStartupXmlIndex(objectname);
    if(itemid > -1) setTableItem(itemid);
}

QString StartupTable::getRandomString(int length)
{
    QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.");

    //shuffle characters
    int pos = possibleCharacters.size() - 1;
    int random;
    while(pos > 1){
        random = qrand() % pos;
        QChar tmp = possibleCharacters.at(random);
        possibleCharacters.replace(random, 1, possibleCharacters.at(pos));
        possibleCharacters.replace(pos, 1, tmp);
        pos--;
    }

    //select characters
    QString randomString;
    for(int i=0; i < length; ++i)
    {
        int index = qrand() % length;
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
