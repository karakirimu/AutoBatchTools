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

#include "profileeditor.h"
#include "ui_profileeditor.h"

ProfileEditor::ProfileEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileEditor)
{
    ui->setupUi(this);

    //dock position
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    //set docktab
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::TabPosition::East);

    //Window data restore
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    QVariant v = settings.value( "profileeditor/geometry" );
    if (v.type() != QVariant::Invalid){
        // load window settings on MainWindow
        restoreGeometry( settings.value( "profileeditor/geometry" ).toByteArray() );
        restoreState( settings.value( "profileeditor/windowState" ).toByteArray() );

        // load window check settings
        ui->actionToolBar->setChecked(settings.value("profileeditor/toolbar", true).toBool());
        ui->actionProcess->setChecked(settings.value("profileeditor/process", true).toBool());
        ui->actionVariant->setChecked(settings.value("profileeditor/variant", true).toBool());
        ui->actionRunSetting->setChecked(settings.value("profileeditor/runset", true).toBool());
        ui->actionRunConsole->setChecked(settings.value("profileeditor/console", true).toBool());

        // load action autohide settings
        bool autohide = settings.value("profileeditor/autohide", true).toBool();
        ui->actionAutohide->setChecked(autohide);
        ui->processDockWidget->setAutohide(autohide);
        ui->setTestDockWidget->setAutohide(autohide);
        ui->consoleDockWidget->setAutohide(autohide);
        ui->variantDockWidget->setAutohide(autohide);
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

    //provide function object
    ui->console->setMultiTask(mlTask);
    ui->consolemessage->setMultiTask(mlTask);

    //function signal bind
    connect(ui->rangeLineEdit, &QLineEdit::textChanged, this, &ProfileEditor::updateRangeText);

    //update window title
    connect(editop, &EditOperator::loadfileChanged, this, &ProfileEditor::onTitleChanged);

    //set window icon
    setWindowIcon(QIcon(":/app_icons/app_pe_24x24.ico"));

    //set radiobutton group id
    ui->searchButtonGroup->setId(ui->variRadioButton, 0);
    ui->searchButtonGroup->setId(ui->fileRadioButton, 1);

    //set menu actions----------------------------------------------------------------------------------

    //file
    connect(ui->actionNew, &QAction::triggered, this, &ProfileEditor::newfileAction);
    connect(ui->actionOpen, &QAction::triggered, this, &ProfileEditor::openAction);
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

    connect(ui->runToolButton, &QToolButton::clicked, this, &ProfileEditor::runTriggered);
    connect(ui->pauseToolButton, &QToolButton::clicked, this, &ProfileEditor::pauseTriggered);
    connect(ui->stopToolButton, &QToolButton::clicked, this, &ProfileEditor::stopTriggered);

    connect(ui->testSettingToolButton, &QToolButton::clicked, this, &ProfileEditor::testSettingTriggered);

    connect(mlTask, &MultiTaskP::processStarted, this, &ProfileEditor::taskStarted);
    connect(mlTask, &MultiTaskP::processPaused, this, &ProfileEditor::taskPaused);
    connect(mlTask, &MultiTaskP::processStopped, this, &ProfileEditor::taskStopped);
    connect(mlTask, &MultiTaskP::processEnd, this, &ProfileEditor::taskEnd);

    connect(ui->actionSettings, &QAction::triggered, this, &ProfileEditor::launchSettingAction);

    //Window
    connect(ui->actionToolBar, &QAction::triggered, ui->mainToolBar, &QWidget::setVisible);
    connect(ui->actionProcess, &QAction::triggered, ui->processDockWidget, &QWidget::setVisible);
    connect(ui->actionVariant, &QAction::triggered, ui->variantDockWidget, &QWidget::setVisible);
    connect(ui->actionRunSetting, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setVisible);
    connect(ui->actionRunConsole, &QAction::triggered, ui->consoleDockWidget, &BaseDockWidget::setVisible);

    connect(ui->processDockWidget, &QDockWidget::visibilityChanged, ui->actionProcess, &QAction::setChecked);
    connect(ui->mainToolBar, &QToolBar::visibilityChanged, ui->actionToolBar, &QAction::setChecked);
    connect(ui->setTestDockWidget, &BaseDockWidget::visibilityChanged, ui->actionRunSetting, &QAction::setChecked);
    connect(ui->consoleDockWidget, &BaseDockWidget::visibilityChanged, ui->actionRunConsole, &QAction::setChecked);

    connect(ui->actionAutohide, &QAction::triggered, ui->processDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->consoleDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->variantDockWidget, &BaseDockWidget::setAutohide);

    //Infomation
    connect(ui->actionAbout, &QAction::triggered, this, &ProfileEditor::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    //end-----------------------------------------------------------------------------------------------

    connect(editop, &EditOperator::processIndexUpdate, this, &ProfileEditor::itemChangedAction);

    //Title * flag
    connect(editop->getUndostack(), &QUndoStack::canUndoChanged, this, &ProfileEditor::onFileEdited);

    initStatusBar();

    //set new file
    newfileAction();
}

//with loading constructor
ProfileEditor::ProfileEditor(QStringList cuiargs, QWidget *parent)
    : ProfileEditor(parent)
{
    QString lfile = cuiargs.last();

    if(cuiargs.count() == 2 && lfile.contains("apro")){
        editop->openAction(lfile);
        initUi();
    }
}

ProfileEditor::~ProfileEditor()
{
    //save window state
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.setValue( "profileeditor/geometry", saveGeometry() );
    settings.setValue( "profileeditor/windowState", saveState() );

    //window menu
    settings.setValue("profileeditor/toolbar", ui->actionToolBar->isChecked());
    settings.setValue("profileeditor/process", ui->actionProcess->isChecked());
    settings.setValue("profileeditor/variant", ui->actionVariant->isChecked());
    settings.setValue("profileeditor/runset", ui->actionRunSetting->isChecked());
    settings.setValue("profileeditor/console", ui->actionRunConsole->isChecked());
    settings.setValue( "profileeditor/autohide", ui->actionAutohide->isChecked());

    delete ui;
    delete settingdialog;
    delete fdialog;
    delete editop;
    delete mlTask;
}

void ProfileEditor::newfileAction()
{
    if(checkOverWrite() == CANCEL) return;

    // not save and newfile generate
    editop->newAction();
    initUi();

}

void ProfileEditor::openAction()
{
    if(checkOverWrite() == CANCEL) return;
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    // open file
    QString fileName = fdialog->getOpenFileName(this,\
                                                tr("Open file"), settings.value("profileeditor/lastopened",\
                                                QDir::currentPath()).toString(), "Profile (*.xml *.apro)");

    if(fileName != ""){
        settings.setValue("profileeditor/lastopened", QFileInfo(fileName).canonicalPath());
        editop->openAction(fileName);
        initUi();
    }

}


// true is save complete, false is not
bool ProfileEditor::saveAction()
{
    // copy only in save action
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    QString fileName =
            fdialog->getSaveFileName(this,\
                                     tr("Save Edit file"),\
                                     settings.value("profileeditor/lastsaved", QDir::currentPath()).toString(),\
                                     "APRO Files (*.apro)");

    if(fileName != ""){
        settings.setValue("profileeditor/lastsaved", QFileInfo(fileName).canonicalPath());
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

    int lastindex = text.lastIndexOf(QRegularExpression(".\\^\\(([0-9]+)((,|)(|[0-9]+|\\w+))+\\)+$"));
    QString rep = text.mid(0,lastindex);

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

    int lastindex = text.lastIndexOf(QRegularExpression(".\\^\\(([0-9]+)((,|)(|[0-9]+|\\w+))+\\)+$"));
    QString rep = text.mid(0,lastindex);

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
    emit editop->processIndexUpdate(editop->getCacheSize() - 1, -1, EditOperator::ADD, EditOperator::MAINEDITOR);

}

void ProfileEditor::deleteAction()
{
    if(dataindexpos <= 1) return;
    editop->deleteAction(dataindexpos);
    emit editop->processIndexUpdate(dataindexpos, -1, EditOperator::DELETE, EditOperator::MAINEDITOR);

    dataindexpos--;
}

void ProfileEditor::cutAction()
{
    if(dataindexpos <= 1) return;
    editop->cutAction(dataindexpos);
    emit editop->processIndexUpdate(dataindexpos, -1, EditOperator::DELETE, EditOperator::MAINEDITOR);

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
    emit editop->processIndexUpdate(dataindexpos, -1, EditOperator::INSERT, EditOperator::MAINEDITOR);
}

void ProfileEditor::upAction()
{
    if(dataindexpos <= 2) return;
    editop->swapAction(dataindexpos, dataindexpos - 1);
    emit editop->processIndexUpdate(dataindexpos - 1, dataindexpos, EditOperator::SWAP, EditOperator::MAINEDITOR);
}

void ProfileEditor::downAction()
{
    if(dataindexpos < 2 || dataindexpos >= (editop->getCacheSize() - 1)) return;
    editop->swapAction(dataindexpos, dataindexpos + 1);
    emit editop->processIndexUpdate(dataindexpos + 1, dataindexpos, EditOperator::SWAP, EditOperator::MAINEDITOR);
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
//    int result = settingdialog->exec();
    settingdialog->open();
}

//QSS_THEME
void ProfileEditor::themeChangeAction()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("pe_general");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
    QFont settingfont = QFont(settings.value("WINDOWFONT", QApplication::font().toString()).toString());
    settingfont.setPointSize(settings.value("WINDOWFONTSIZE", QApplication::font().pointSize()).toInt());
    settings.endGroup();

    if(stylecolor != "Default"){
#ifdef QT_DEBUG
        QFile file(QString("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/%1.qss").arg(stylecolor));

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
                                         tr("Save Edit file"),\
                                         QDir::currentPath(),\
                                         "APRO Files (*.apro)");

        if(fileName != "") editop->saveAction(fileName);
    }else{
        editop->saveAction(editop->getLoadfile());
    }
}

void ProfileEditor::exportAction()
{
    QString fileName =
            fdialog->getSaveFileName(this,\
                                     tr("Export XML file"),\
                                     QDir::currentPath(),\
                                     "XML Files (*.xml)");
    editop->exportAction(fileName);
}

void ProfileEditor::onTitleChanged(QString newload)
{
    QString newtitle;
    QFileInfo info(newload);
    if(info.suffix() == "autosave"){
        newtitle = tr("untitled");
    }else{
        newtitle = info.fileName();
    }

    setWindowTitle(newtitle + " - ProfileEditor");

}

void ProfileEditor::onFileEdited(bool edited)
{
    QString title = this->windowTitle();
//    editop->getUndostack()->index()

    lastedited = edited;
    if(edited){
        //add * first
        if(title.at(0) != "*") setWindowTitle("*" + title);

    }else{
        //remove first *
        if(title.at(0) == "*") setWindowTitle(title.remove(0,1));
    }
}

void ProfileEditor::itemChangedAction(int after, int before, int function, int sendfrom)
{
    Q_UNUSED(before)
    Q_UNUSED(sendfrom)

    int showdata = 1;

    if(function == EditOperator::DELETE){
        showdata = (after > 1)? after - 1 : 1;

    }else {
        showdata = (after > 1)? after : 1;
    }

    qDebug() << "[ProfileEditor::itemChangedAction]   rowpos :" << after;

    //update show
    emit statusLabelChanged(tr("Process selected: No. %1").arg(showdata));

    //update row position
    dataindexpos = after;
}

void ProfileEditor::about()
{
    AboutPE *ab = new AboutPE;
    ab->setStyleSheet(this->styleSheet());
    ab->move(this->geometry().center() - ab->rect().center());
    ab->show();

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
        mlTask->setRange(key, ui->rangeLineEdit->text());

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

void ProfileEditor::testSettingTriggered()
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

    QString rep = text.remove(QRegularExpression(".\\^\\(([0-9]+)((,|)(|[0-9]+|\\w+))+\\)+$"));
    ui->actionUndo->setText(tr("Undo %1").arg(rep));
}

void ProfileEditor::onRedoTextChanged(QString text)
{
    //remove operation
#ifdef QT_DEBUG
    qDebug() << "redo count : " << editop->getUndostack()->count() << "raw data : " << text;
#endif

    QString rep = text.remove(QRegularExpression(".\\^\\(([0-9]+)((,|)(|[0-9]+|\\w+))+\\)+$"));
    ui->actionRedo->setText(tr("Redo %1").arg(rep));
}

void ProfileEditor::closeEvent(QCloseEvent *event)
{
    // check file difference (0: same file generated)
//    int fileedit = sfunction->checkFileEdited();
//    qDebug() << "closeEvent::checkdiff" << fileedit;

    if(!lastedited) return;

    // ドキュメントが変更されている場合の警告
    QMessageBox::StandardButton res = QMessageBox::question(this, tr("Alert"),\
                                    tr("File was edited.\nDo you want to save this file ?"),\
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:    overWriteSaveAction(); break;
    case QMessageBox::No:     editop->abortAction(); break;
    case QMessageBox::Cancel: event->ignore();       break;
    default:                                         break;
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

//when opening new file
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

void ProfileEditor::setRunButtonState(bool run, bool pause, bool stop)
{
    ui->actionRun->setEnabled(run);
    ui->actionPause->setEnabled(pause);
    ui->actionStop->setEnabled(stop);
    ui->runToolButton->setEnabled(run);
    ui->pauseToolButton->setEnabled(pause);
    ui->stopToolButton->setEnabled(stop);
}

int ProfileEditor::checkOverWrite()
{
    if(!lastedited) return 3;

    // Warning if document is edited by user.
    QMessageBox::StandardButton res = QMessageBox::question(this, tr("Alert"),\
                                    tr("File was edited.\nDo you want to save this file ?"),\
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);


    switch( res )
    {
    case QMessageBox::Yes: overWriteSaveAction(); return 0;
    case QMessageBox::No:                         return 1;
    case QMessageBox::Cancel:                     return 2;
    default:                                      return 2;
    }
}
