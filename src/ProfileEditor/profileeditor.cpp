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

#include "profileeditor.h"
#include "ui_profileeditor.h"

ProfileEditor::ProfileEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileEditor)
{
    qDebug() << "[ProfileEditor::ProfileEditor] default constructor";

    ui->setupUi(this);

    //dock position
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    //set docktab
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::TabPosition::East);

    //Window data restore
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    QVariant v = settings.value( sc.GEOMETRY );
    bool toolbartexthide = false;

    if (v.metaType().isValid()){

        // load window settings on MainWindow
        restoreGeometry( settings.value( sc.GEOMETRY ).toByteArray() );
        restoreState( settings.value( sc.WINDOW_STATE ).toByteArray() );

        // load window check settings
        ui->actionToolBarEdit->setChecked(settings.value(sc.SHOW_TOOLBAR_EDIT, true).toBool());

        ui->actionProcess->setChecked(settings.value(sc.SHOW_PROCESS, true).toBool());
        ui->actionVariant->setChecked(settings.value(sc.SHOW_VARIANT, true).toBool());
        ui->actionRunSetting->setChecked(settings.value(sc.SHOW_TEST, true).toBool());
        ui->actionRunConsole->setChecked(settings.value(sc.SHOW_CONSOLE, true).toBool());

        // load action autohide settings
        bool autohide = settings.value(sc.HIDE_TITLEBAR, true).toBool();
        ui->actionAutohide->setChecked(autohide);
        ui->processDockWidget->setAutohide(autohide);
        ui->setTestDockWidget->setAutohide(autohide);
        ui->consoleDockWidget->setAutohide(autohide);
        ui->variantDockWidget->setAutohide(autohide);

        // toolbar text settings
        toolbartexthide = settings.value(sc.HIDE_TOOLBARTEXT, false).toBool();
        ui->actionToolBarHideDesc->setChecked(toolbartexthide);

        // load recent opened files
        setOpenRecent();
    }

    //set Attrs
    settingdialog = new SettingDialog();
    fdialog = new QFileDialog();

    //theme settings
    themeChangeAction();

    //init sharedfunction
    editop = new EditOperator(this);
    mlTask = new MultiTaskP();

    //connection set
    ui->editorTab->setConnection();

    //provide operator
    ui->flowTableWidget->setEditOperator(editop);
    ui->editorTab->setEditOperator(editop);
    ui->innerStackedWidget->setEditOperator(editop);
    ui->variantTreeWidget->setEditOperator(editop);

    //test
//    ui->graphicsView->hide();
//    qDebug() << "profileeditor" << geometry().center();

//    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //provide function object
    ui->console->setMultiTask(mlTask);
    ui->consolemessage->setMultiTask(mlTask);

    //update window title
    connect(editop, &EditOperator::loadfileChanged, this, &ProfileEditor::reloadWindow);

    //set window icon
    setWindowIcon(QIcon(":/app_icons/app_pe.ico"));

    //set radiobutton group id
    ui->searchButtonGroup->setId(ui->variRadioButton, 0);
    ui->searchButtonGroup->setId(ui->fileRadioButton, 1);

    //set menu actions----------------------------------------------------------------------------------

    //file
    connect(ui->actionNew, &QAction::triggered, this, &ProfileEditor::newfileAction);
    connect(ui->actionOpen, &QAction::triggered, this, &ProfileEditor::openAction);

    connect(this, &ProfileEditor::openRecentClicked, this, &ProfileEditor::openRecentAction);
    connect(ui->actionClearRecentHistory, &QAction::triggered, this, &ProfileEditor::clearOpenRecentAction);
    connect(ui->actionOpenLastClosed, &QAction::triggered, this, &ProfileEditor::openLastClosedAction);

    connect(ui->actionSave, &QAction::triggered, this, &ProfileEditor::saveAction);
    connect(ui->actionOverWriteSave, &QAction::triggered, this, &ProfileEditor::overWriteSaveAction);
//    connect(ui->actionCurrentSave, &QAction::triggered, this, &ProfileEditor::saveCurrentAction);
    connect(ui->actionExport, &QAction::triggered, this, &ProfileEditor::exportAction);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

    //Edit
    connect(ui->actionUndo, &QAction::triggered, this, &ProfileEditor::undoAction);
    connect(ui->actionRedo, &QAction::triggered, this, &ProfileEditor::redoAction);
    connect(ui->actionAddItem, &QAction::triggered, this, &ProfileEditor::addAction);
    connect(ui->actionDeleteItem, &QAction::triggered, this, &ProfileEditor::deleteAction);
    connect(ui->actionCut, &QAction::triggered, this, &ProfileEditor::cutAction);
    connect(ui->actionCopy, &QAction::triggered, this, &ProfileEditor::copyAction);
    connect(ui->actionPaste, &QAction::triggered, this, &ProfileEditor::pasteAction);
    connect(ui->actionUpItem, &QAction::triggered, this, &ProfileEditor::upAction);
    connect(ui->actionDownItem, &QAction::triggered, this, &ProfileEditor::downAction);
    connect(ui->actionReloadProcess, &QAction::triggered, this, &ProfileEditor::reloadAction);

    //Edit (Undo,Redo)
    connect(editop->getUndostack(), &QUndoStack::canUndoChanged, ui->actionUndo, &QAction::setEnabled);
    connect(editop->getUndostack(), &QUndoStack::canRedoChanged, ui->actionRedo, &QAction::setEnabled);
    connect(editop->getUndostack(), &QUndoStack::undoTextChanged, this, &ProfileEditor::onUndoTextChanged);
    connect(editop->getUndostack(), &QUndoStack::redoTextChanged, this, &ProfileEditor::onRedoTextChanged);

    //Tools
    connect(ui->actionRun, &QAction::triggered,this, &ProfileEditor::runTriggered);
    connect(ui->actionPause, &QAction::triggered, this, &ProfileEditor::pauseTriggered);
    connect(ui->actionStop, &QAction::triggered, this, &ProfileEditor::stopTriggered);

//    connect(ui->runToolButton, &QToolButton::clicked, this, &ProfileEditor::runTriggered);
//    connect(ui->pauseToolButton, &QToolButton::clicked, this, &ProfileEditor::pauseTriggered);
//    connect(ui->stopToolButton, &QToolButton::clicked, this, &ProfileEditor::stopTriggered);

//    connect(ui->testSettingToolButton, &QToolButton::clicked, this, &ProfileEditor::testSettingTriggered);

    connect(mlTask, &MultiTaskP::processStarted, this, &ProfileEditor::taskStarted);
    connect(mlTask, &MultiTaskP::processPaused, this, &ProfileEditor::taskPaused);
    connect(mlTask, &MultiTaskP::processStopped, this, &ProfileEditor::taskStopped);
    connect(mlTask, &MultiTaskP::processEnd, this, &ProfileEditor::taskEnd);

    connect(ui->actionSettings, &QAction::triggered, this, &ProfileEditor::launchSettingAction);

    //Window
    connect(ui->actionToolBarEdit, &QAction::triggered, ui->mainToolBar, &QWidget::setVisible);
    connect(ui->actionProcess, &QAction::triggered, ui->processDockWidget, &QWidget::setVisible);
    connect(ui->actionVariant, &QAction::triggered, ui->variantDockWidget, &QWidget::setVisible);
    connect(ui->actionRunSetting, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setVisible);
    connect(ui->actionRunConsole, &QAction::triggered, ui->consoleDockWidget, &BaseDockWidget::setVisible);

    connect(ui->processDockWidget, &QDockWidget::visibilityChanged, ui->actionProcess, &QAction::setChecked);
    connect(ui->mainToolBar, &QToolBar::visibilityChanged, ui->actionToolBarEdit, &QAction::setChecked);
    connect(ui->setTestDockWidget, &BaseDockWidget::visibilityChanged, ui->actionRunSetting, &QAction::setChecked);
    connect(ui->consoleDockWidget, &BaseDockWidget::visibilityChanged, ui->actionRunConsole, &QAction::setChecked);
    connect(ui->variantDockWidget, &BaseDockWidget::visibilityChanged, ui->actionVariant, &QAction::setChecked);

    connect(ui->actionAutohide, &QAction::triggered, ui->processDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->consoleDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->variantDockWidget, &BaseDockWidget::setAutohide);

    connect(ui->actionToolBarHideDesc, &QAction::triggered, this, &ProfileEditor::hideToolBarDesc);

    //Infomation
    connect(ui->actionAbout, &QAction::triggered, this, &ProfileEditor::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    //end-----------------------------------------------------------------------------------------------

    connect(editop, &EditOperator::processIndexUpdate, this, &ProfileEditor::itemChangedAction);

    // Title * flag
    connect(editop->getUndostack(), &QUndoStack::canUndoChanged, this, &ProfileEditor::onFileEdited);

    // set additional toobar
    initRunRangeToolBar();
    initRunToolBar();
    hideToolBarDesc(toolbartexthide);

    // set statusbar
    initStatusBar();
}

//with loading constructor
ProfileEditor::ProfileEditor(QStringList cuiargs, QWidget *parent)
    : ProfileEditor(parent)
{
    qDebug() << "[ProfileEditor::ProfileEditor] arguments constructor";

    QString lfile = cuiargs.last();

    if(cuiargs.count() == 2 && lfile.contains("apro")){
        editop->openAction(lfile);
    }else{
        // it must be zero.
        QTimer::singleShot(0, this, &ProfileEditor::initFileLoad);
    }
}

ProfileEditor::~ProfileEditor()
{
    //save window state
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    settings.setValue( sc.GEOMETRY, saveGeometry() );
    settings.setValue( sc.WINDOW_STATE, saveState() );

    //window menu
    settings.setValue(sc.SHOW_TOOLBAR_EDIT, ui->actionToolBarEdit->isChecked());
    settings.setValue(sc.SHOW_TOOLBAR_RUN, actionToolBarRun->isChecked());
    settings.setValue(sc.SHOW_TOOLBAR_RANGE, actionToolBarTestRange->isChecked());

    settings.setValue(sc.SHOW_PROCESS, ui->actionProcess->isChecked());
    settings.setValue(sc.SHOW_VARIANT, ui->actionVariant->isChecked());
    settings.setValue(sc.SHOW_TEST, ui->actionRunSetting->isChecked());
    settings.setValue(sc.SHOW_CONSOLE, ui->actionRunConsole->isChecked());
    settings.setValue(sc.HIDE_TITLEBAR, ui->actionAutohide->isChecked());

    settings.setValue(sc.HIDE_TOOLBARTEXT, ui->actionToolBarHideDesc->isChecked());

    delete actionRunSetting;
    delete runToolBar;
    delete actionToolBarRun;

    delete rangeLineEdit;
    delete testRangeToolBar;
    delete actionToolBarTestRange;

    delete ui;

    delete settingdialog;
    delete fdialog;
    delete editop;
    delete mlTask;
}

void ProfileEditor::showEvent(QShowEvent *event)
{
    static bool showfirst = true;

    if(!showfirst){
        event->ignore();
        return;
    }

    showfirst = false;
    event->accept();
}

void ProfileEditor::newfileAction()
{
    if(checkOverWrite() == CANCEL) return;

    // not save and newfile generate
    editop->newAction();
}

void ProfileEditor::openAction()
{
    if(checkOverWrite() == CANCEL) return;
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    // open file
    QString fileName = fdialog->getOpenFileName(this,\
                                                tr("Open Profile"), settings.value(sc.ABE_RECENTLY_OPENED,\
                                                QDir::currentPath()).toString(), "Profile (*.xml *.apro)");

    if(fileName != ""){
        // save last opened folder
        settings.setValue(sc.ABE_RECENTLY_OPENED, QFileInfo(fileName).canonicalPath());
        editop->openAction(fileName);

        // reset recent open menu
        clearOpenRecent(false);
        setOpenRecent();
    }

}

void ProfileEditor::openRecentAction(QString filename)
{
    if(checkOverWrite() == CANCEL) return;
    editop->openAction(filename);
}

void ProfileEditor::clearOpenRecentAction()
{
    clearOpenRecent(true);
    ui->menuOpenRecent->setEnabled(false);
}

void ProfileEditor::openLastClosedAction()
{
    if(checkOverWrite() == CANCEL) return;
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    QStringList list = settings.value(sc.ABE_RECENT_FILES).value<QStringList>();
    editop->openAction(list.first());
}


// true is save complete, false is not
bool ProfileEditor::saveAction()
{
    // copy only in save action
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    QString fileName =
            fdialog->getSaveFileName(this,\
                                     tr("Save Profile"),\
                                     settings.value(sc.ABE_LAST_SAVED_DIR, QDir::currentPath()).toString(),\
                                     "Profile (*.apro)");

    if(fileName != ""){
        settings.setValue(sc.ABE_LAST_SAVED_DIR, QFileInfo(fileName).canonicalPath());
        editop->saveAction(fileName);
    }else{
        return false;
    }
    return true;
}

void ProfileEditor::undoAction()
{
    if(!editop->getUndostack()->canUndo()) return;

    editop->getUndostack()->undo();

    // update ui element
    QString text = editop->getUndostack()->redoText();

    //int lastindex = text.lastIndexOf(QRegularExpression(".\\^\\(([0-9]+)((,|)(|[0-9]+|\\w+))+\\)+$"));
    qsizetype lastindex = text.lastIndexOf(QRegularExpression(".\\^\\((,|\\w+|\\S){1,}\\)+$"));

    // +3 means string of " ^(", -1 means string of ")";
    QString updop = text.mid(lastindex + 3, text.length() - lastindex - 4);

    ui->innerStackedWidget->updateIndex(updop);
    ui->flowTableWidget->updateIndex(updop);
    ui->editorTab->updateIndex(updop);
    ui->variantTreeWidget->updateIndex(updop);
}

void ProfileEditor::redoAction()
{
    if(!editop->getUndostack()->canRedo()) return;

    editop->getUndostack()->redo();

    // update ui element
    QString text = editop->getUndostack()->undoText();

    //int lastindex = text.lastIndexOf(QRegularExpression(".\\^\\(([0-9]+)((,|)(|[0-9]+|\\w+))+\\)+$"));
    qsizetype lastindex
        = text.lastIndexOf(QRegularExpression(".\\^\\((,|\\w+|\\S){1,}\\)+$"));

    // +3 means string of " ^(", -1 means string of ")";
    QString updop = text.mid(lastindex + 3, text.length() - lastindex - 4);

    ui->innerStackedWidget->updateIndex(updop);
    ui->flowTableWidget->updateIndex(updop);
    ui->editorTab->updateIndex(updop);
    ui->variantTreeWidget->updateIndex(updop);

}

void ProfileEditor::addAction()
{
    editop->addAction();
    emit editop->processIndexUpdate(editop->getCacheSize() - 1, -1, EditOperator::ADD);

}

void ProfileEditor::deleteAction()
{
    if(dataindexpos <= 1) return;
    editop->deleteAction(dataindexpos);
    emit editop->processIndexUpdate(dataindexpos, -1, EditOperator::DELETE);

    dataindexpos--;
}

void ProfileEditor::cutAction()
{
    if(dataindexpos <= 1) return;
    editop->cutAction(dataindexpos);
    emit editop->processIndexUpdate(dataindexpos, -1, EditOperator::DELETE);

    dataindexpos--;
}

void ProfileEditor::copyAction()
{
    if(dataindexpos > 1) editop->copyAction(dataindexpos);
}

void ProfileEditor::pasteAction()
{
    if(dataindexpos <= 1) return;
    editop->pasteAction(dataindexpos);
    emit editop->processIndexUpdate(dataindexpos, -1, EditOperator::INSERT);
}

void ProfileEditor::upAction()
{
    if(dataindexpos <= 2) return;
    editop->swapAction(dataindexpos, dataindexpos - 1);
    emit editop->processIndexUpdate(dataindexpos - 1, dataindexpos, EditOperator::SWAP);
}

void ProfileEditor::downAction()
{
    if(dataindexpos < 2 || dataindexpos >= (editop->getCacheSize() - 1)) return;
    editop->swapAction(dataindexpos, dataindexpos + 1);
    emit editop->processIndexUpdate(dataindexpos + 1, dataindexpos, EditOperator::SWAP);
}

void ProfileEditor::reloadAction()
{
    ui->flowTableWidget->reloadAction();
}

void ProfileEditor::launchSettingAction()
{
    //setup settingdialog
    settingdialog->move(this->geometry().center() - settingdialog->rect().center());
    settingdialog->setListPos(SettingDialog::GENERAL);
    settingdialog->open();
}

//QSS_THEME
void ProfileEditor::themeChangeAction()
{
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    //theme settings
    settings.beginGroup(sc.GROUP_ABE);
    QString stylecolor = settings.value(sc.ABE_THEME, "Default").toString();
    QFont settingfont = QFont(settings.value(sc.ABE_FONT, QApplication::font().toString()).toString());
    settingfont.setPointSize(settings.value(sc.ABE_FONTSIZE, QApplication::font().pointSize()).toInt());
    settings.endGroup();

    if(stylecolor != "Default"){
#ifdef QT_DEBUG
#ifdef Q_OS_WIN
        QFile file(QString("../../res/themes/%1.qss").arg(stylecolor));
#else
        QFile file(QString("../AutoBatchRunner-master/res/themes/%1.qss").arg(stylecolor));
#endif

#else
        QFile file(QString(":/themes/%1.qss").arg(stylecolor));
#endif
        if(file.open( QFile::ReadOnly | QFile::Text )){
            QString data(QLatin1String(file.readAll()));
            this->setStyleSheet(data);
            settingdialog->setStyleSheet(data);
            ui->searchComboBox->setStyleSheet(data);
            ui->pluginComboBox->setStyleSheet(data);
            ui->searchInputComboBox->setStyleSheet(data);
            ui->fileTableWidget->setStyleSheet(data);
        }
    }

    QApplication::setFont(settingfont);
}


void ProfileEditor::overWriteSaveAction()
{
    if(editop->getLoadfile().contains(".autosave")) {
        QString fileName =
                fdialog->getSaveFileName(this,\
                                         tr("Save Profile"),\
                                         QDir::currentPath(),\
                                         "APRO File (*.apro)");

        if(fileName != "") editop->saveAction(fileName);
    }else{
        editop->saveAction(editop->getLoadfile());
    }
}

void ProfileEditor::exportAction()
{
    QString fileName =
            fdialog->getSaveFileName(this,\
                                     tr("Export"),\
                                     QDir::currentPath(),\
                                     "XML File (*.xml)");
    editop->exportAction(fileName);
}

void ProfileEditor::reloadWindow(QString newload)
{
    QString newtitle;
    QFileInfo info(newload);
    lastedited = false;

    if(info.suffix() == "autosave"){
        newtitle = tr("Untitled");
    }else{
        newtitle = info.fileName();
    }

    setWindowTitle(newtitle + " - AutoBatchEditor");

    initUi();
}

void ProfileEditor::onFileEdited(bool edited)
{
    QString title = this->windowTitle();
//    editop->getUndostack()->index()

    lastedited = edited;
    if(edited){
        //add * first
        if(title.at(0) != '*') setWindowTitle("*" + title);

    }else{
        //remove first *
        if(title.at(0) == '*') setWindowTitle(title.remove(0,1));
    }
}

void ProfileEditor::itemChangedAction(int after, int before, int function)
{
    Q_UNUSED(before)


    int showdata = 1;

    if(function == EditOperator::DELETE){
        showdata = (after > 1)? after - 1 : 1;

    }else {
        showdata = (after > 1)? after : 1;
    }

    qDebug() << "[ProfileEditor::itemChangedAction]   rowpos :" << after;

    //update show
    emit statusLabelChanged(tr("No. %1 selected").arg(showdata));

    //update row position
    dataindexpos = after;
}

void ProfileEditor::about()
{
    AboutABE *ab = new AboutABE;
    ab->setStyleSheet(this->styleSheet());
    ab->move(this->geometry().center() - ab->rect().center());
    ab->show();

}

void ProfileEditor::initRunToolBar()
{
    runToolBar = new QToolBar(tr("Run"));
    runToolBar->setObjectName("RunToolBar");
    runToolBar->setToolButtonStyle(ui->mainToolBar->toolButtonStyle());

    runToolBar->addAction(ui->actionRun);
    runToolBar->addAction(ui->actionPause);
    runToolBar->addAction(ui->actionStop);

    actionRunSetting = new QAction(QIcon(":/default_icons/string.png"), tr("Test"));
    actionRunSetting->setToolTip(tr("Test Settings"));
    connect(actionRunSetting, &QAction::triggered, this, &ProfileEditor::runTestSettingsTriggered);
    runToolBar->addAction(actionRunSetting);

    this->addToolBar(runToolBar);

    // add menu action
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    actionToolBarRun = new QAction(runToolBar->windowTitle());
    actionToolBarRun->setCheckable(true);
    actionToolBarRun->setChecked(settings.value(sc.SHOW_TOOLBAR_RUN, true).toBool());
    connect(actionToolBarRun, &QAction::triggered, runToolBar, &QToolBar::setVisible);
//    ui->menuToolBar->addAction(actionToolBarRun);
    ui->menuToolBar->insertAction(ui->actionToolBarEdit, actionToolBarRun);

    runToolBar->setVisible(actionToolBarRun->isChecked());
}

void ProfileEditor::initRunRangeToolBar()
{
    testRangeToolBar = new QToolBar(tr("Test Range"));
    testRangeToolBar->setObjectName("TestRangeToolBar");
    testRangeToolBar->setToolButtonStyle(ui->mainToolBar->toolButtonStyle());

    rangeLineEdit = new QLineEdit();
    rangeLineEdit->setMinimumWidth(150);
    rangeLineEdit->setMaximumWidth(200);
    rangeLineEdit->setPlaceholderText(tr("Enter the process test range here..."));
    rangeLineEdit->setToolTip(tr("Specify the process test range.\nExample: 0-7 0, 1, 2, 3"));
    connect(rangeLineEdit, &QLineEdit::textChanged, this, &ProfileEditor::updateRangeText);
    testRangeToolBar->addWidget(rangeLineEdit);
    this->addToolBar(testRangeToolBar);

    // add menu action
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    actionToolBarTestRange = new QAction(testRangeToolBar->windowTitle());
    actionToolBarTestRange->setCheckable(true);
    actionToolBarTestRange->setChecked(settings.value(sc.SHOW_TOOLBAR_RANGE, true).toBool());
    connect(actionToolBarTestRange, &QAction::triggered, testRangeToolBar, &QToolBar::setVisible);
//    ui->menuToolBar->addAction(actionToolBarTestRange);
    ui->menuToolBar->insertAction(ui->actionToolBarEdit, actionToolBarTestRange);

    testRangeToolBar->setVisible(actionToolBarTestRange->isChecked());
}

void ProfileEditor::taskStarted(QString objectname, int runfrom)
{
    qDebug() << "[ProfileEditor::taskStarted]";
    Q_UNUSED(objectname)
    Q_UNUSED(runfrom)

    setRunButtonState(false, true, true);
}

void ProfileEditor::taskPaused(QString objectname)
{
    qDebug() << "[ProfileEditor::taskPaused]";
    Q_UNUSED(objectname)

    setRunButtonState(true, false, true);
}

// deleting target
void ProfileEditor::taskStopped(QString objectname)
{
    qDebug() << "[ProfileEditor::taskStopped]";
    Q_UNUSED(objectname)

    setRunButtonState(true, false, false);
}

void ProfileEditor::taskEnd(QString objectname, int runfrom)
{
    qDebug() << "[ProfileEditor::taskEnd]";
    Q_UNUSED(runfrom)

    setRunButtonState(true, false, false);


    mlTask->removeTask(objectname);
    key = "";
    ui->console->setReadObjectName(objectname);
    ui->consolemessage->setObjectName(objectname);
}

void ProfileEditor::runTriggered()
{
    int state = checkOverWrite();
    if(state == CANCEL || state == NO) return;

    setRunButtonState(false, false, false);


    qDebug() << "[ProfileEditor::runTriggered]";

    if(key == ""){
        key = mlTask->generateRandom(32);

        mlTask->addTask(key, editop->getLoadfile());
        ui->console->setReadObjectName(key);
        ui->consolemessage->setObjectName(key);

        //get filelist
        QStringList flist;
        int filecount = ui->fileTableWidget->rowCount();
        for(int i = 0; i < filecount; i++){
            flist.append(ui->fileTableWidget->item(i,0)->text());
        }

        mlTask->setInputFileList(key, &flist);

        //init execute list range
        mlTask->setRange(key, rangeLineEdit->text());

        qDebug() << "key:: " << key;

    }else{
        mlTask->processPause(key);
    }
}

void ProfileEditor::pauseTriggered()
{
    setRunButtonState(false, false, false);
    mlTask->processPause(key);
}

void ProfileEditor::stopTriggered()
{
    setRunButtonState(true, false, false);

    mlTask->removeTask(key);
    key = "";
    ui->console->setReadObjectName(key);
    ui->consolemessage->setObjectName(key);
}

void ProfileEditor::hideToolBarDesc(bool state)
{
    if(state){
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        runToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }else{
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        runToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
}

void ProfileEditor::runTestSettingsTriggered()
{
    //setup settingdialog
    settingdialog->move(this->geometry().center() - settingdialog->rect().center());
    settingdialog->setListPos(SettingDialog::TEST);
    settingdialog->show();
}

void ProfileEditor::updateRangeText(QString range)
{
    if(key != "") return;
    mlTask->setRange(key, range);
}

void ProfileEditor::onUndoTextChanged(QString text)
{
    //remove operation
#ifdef QT_DEBUG
    qDebug() << "undo count : " << editop->getUndostack()->count() << "raw data : " << text;
#endif

    QString rep = text.remove(QRegularExpression(".\\^\\((,|\\w+|\\S){1,}\\)+$"));
    ui->actionUndo->setText(tr("Undo %1").arg(rep));
}

void ProfileEditor::onRedoTextChanged(QString text)
{
    //remove operation
#ifdef QT_DEBUG
    qDebug() << "redo count : " << editop->getUndostack()->count() << "raw data : " << text;
#endif

    QString rep = text.remove(QRegularExpression(".\\^\\((,|\\w+|\\S){1,}\\)+$"));
    ui->actionRedo->setText(tr("Redo %1").arg(rep));
}

void ProfileEditor::closeEvent(QCloseEvent *event)
{
    // check file difference (0: same file generated)
//    int fileedit = sfunction->checkFileEdited();
//    qDebug() << "closeEvent::checkdiff" << fileedit;

    if(!lastedited) return;

    // Warning if the document has changed
    QMessageBox::StandardButton res = QMessageBox::question(this, tr("Alert"),\
                                    tr("Profile was edited.\nDo you want to save this?"),\
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:    overWriteSaveAction(); break;
    case QMessageBox::No:     editop->abortAction(); break;
    case QMessageBox::Cancel: event->ignore();       break;
    default:                                         break;
    }
}

void ProfileEditor::initFileLoad()
{
    // search *.autosave file and recover
    if(!checkAutoSave()){
        // set new file
        newfileAction();
    }
}

void ProfileEditor::initStatusBar()
{
    QLabel *statusLabel = new QLabel();
    statusLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    ui->statusBar->addPermanentWidget(statusLabel, 3);
    connect(this, &ProfileEditor::statusLabelChanged, statusLabel, &QLabel::setText);

    QProgressBar *progressbar = new QProgressBar();
    progressbar->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(progressbar, 1);
    connect(mlTask, &MultiTaskP::processInitCount, progressbar, &QProgressBar::setRange);
    connect(mlTask, &MultiTaskP::processCurrent, progressbar, &QProgressBar::setValue);
}

/**
 * @fn ProfileEditor::initUi
 * @brief Reset UI when opening a new file.
 */
void ProfileEditor::initUi()
{
    //reload file
    this->blockSignals(true);

    ui->innerStackedWidget->reloadAction();
    ui->flowTableWidget->reloadAction();
    ui->variantTreeWidget->reloadAction();

    this->blockSignals(false);

    //reset tree row position
    dataindexpos = 0;

}

/**
 * @fn ProfileEditor::clearOpenRecent
 * @brief reset recent files list
 * @param deletelist Whether to delete saved recently opened lists. true is deleted, false is not.
 */
void ProfileEditor::clearOpenRecent(bool deletelist)
{
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    QStringList list = settings.value(sc.ABE_RECENT_FILES).value<QStringList>();

    QList<QAction *> acts = ui->menuOpenRecent->actions();

    for (QAction *act : acts) {
        for(const QString &recentfile : list) {
            if(act->text() == recentfile){
                ui->menuOpenRecent->removeAction(act);
                break;
            }
        }
    }

    if(deletelist) settings.setValue(sc.ABE_RECENT_FILES, QStringList());
}

/**
 * @fn ProfileEditor::setOpenRecent
 * @brief Create 'Open Recent' menu actions.
 */
void ProfileEditor::setOpenRecent()
{
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    QStringList list = settings.value(sc.ABE_RECENT_FILES).value<QStringList>();

    for (const QString& file : list) {
        QAction *action = new QAction(file);
        connect(action, &QAction::triggered, [=](){
            emit openRecentClicked(file);
        });
        ui->menuOpenRecent->insertAction(ui->actionClearRecentHistory, action);
    }
    ui->menuOpenRecent->insertSeparator(ui->actionClearRecentHistory);
    if(list.count() > 0) ui->menuOpenRecent->setEnabled(true);
}

void ProfileEditor::setRunButtonState(bool run, bool pause, bool stop)
{
    ui->actionRun->setEnabled(run);
    ui->actionPause->setEnabled(pause);
    ui->actionStop->setEnabled(stop);
//    ui->runToolButton->setEnabled(run);
//    ui->pauseToolButton->setEnabled(pause);
//    ui->stopToolButton->setEnabled(stop);
}

int ProfileEditor::checkOverWrite()
{
    if(!lastedited) return NOTNEED;

    // Warning if document is edited by user.
    QMessageBox::StandardButton res = QMessageBox::question(this, tr("Alert"),\
                                    tr("Profile was edited.\nDo you want to save this?"),\
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);


    switch( res )
    {
    case QMessageBox::Yes: overWriteSaveAction(); return YES;
    case QMessageBox::No:                         return NO;
    case QMessageBox::Cancel:
    default:                                      return CANCEL;
    }
}

/**
 * @fn ProfileEditor::checkAutoSave
 * @brief search *.autosave file and recover
 * @return Returns true if the user has selected an autosave file, false otherwise.
 */
bool ProfileEditor::checkAutoSave()
{
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    QStringList slist = QStringList() << "*.autosave";

    QStringList result;
    BaseFileSearch search;
    result = search.listFiles(settings.value(sc.ABE_AUTOSAVE_DIR, "./").toString(), &slist);

    // get filtered result
    search.setRegularExpressionCondition(&result, "(\\.~\\$new_\\w+\\.autosave)$");

    if(result.count() > 0){
        AutoSaveSelectionDialog autosaveDialog;
        autosaveDialog.setStyleSheet(this->styleSheet());
        autosaveDialog.move(QPoint(this->geometry().center().x(), this->geometry().center().y()) \
                            - QPoint(autosaveDialog.rect().center().x(), autosaveDialog.rect().center().y()));
        autosaveDialog.setAutoSaveFileList(&result);

        QString selectfile = autosaveDialog.execDialog();
        if(selectfile != ""){
            editop->autoSaveRecoveryAction(selectfile);
            onFileEdited(true);
            return true;
        }
    }

    return false;
}
