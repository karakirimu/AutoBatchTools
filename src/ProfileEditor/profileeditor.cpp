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
    tabifyDockWidget(ui->globalVariantDockWidget, ui->localVariantDockWidget);
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::TabPosition::East);
    ui->localVariantDockWidget->show();

    //Window data restore
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    QVariant v = settings.value( "profileeditor/geometry" );
    if (v.type() != QVariant::Invalid){
        // load window settings on MainWindow
        restoreGeometry( settings.value( "profileeditor/geometry" ).toByteArray() );
        restoreState( settings.value( "profileeditor/windowState" ).toByteArray() );
    }

    //set Attrs
    settingdialog = new SettingDialog();
    fdialog = new QFileDialog();

    //theme settings
    themeChangeAction();

    //set dock autohide TODO: not saved menu
    ui->processDockWidget->setAutohide(ui->actionAutohide->isChecked());
//    ui->editorDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->globalVariantDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->localVariantDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->setTestDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->consoleDockWidget->setAutohide(ui->actionAutohide->isChecked());

    //init sharedfunction
    editop = new EditOperator(this);
    mlTask = new MultiTaskP();

    //connection set
    ui->editorTab->setConnection();

    //provide operator
//    ui->runTreeWidget->setEditOperator(editop);
//    ui->graphicsView->setEditOperator(editop);
    ui->flowTableWidget->setEditOperator(editop);
    ui->variantTableWidget->setEditOperator(editop);
    ui->editorTab->setEditOperator(editop);
    ui->innerStackedWidget->setEditOperator(editop);

    //test
//    ui->graphicsView->hide();
//    qDebug() << "profileeditor" << geometry().center();

    //update tree position
//    connect(editop, &EditOperator::ui_selectindexUpdate, this, &ProfileEditor::setTreerowpos_select);
//    connect(editop, &EditOperator::ui_funcindexUpdate, this, &ProfileEditor::setTreerowpos_update);

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
    connect(ui->actionUpItem, SIGNAL(triggered()), this, SLOT(upAction()));
    connect(ui->actionDownItem, SIGNAL(triggered()), this, SLOT(downAction()));

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

    connect(mlTask, &MultiTaskP::processStarted, this, &ProfileEditor::taskStarted);
    connect(mlTask, &MultiTaskP::processPaused, this, &ProfileEditor::taskPaused);
    connect(mlTask, &MultiTaskP::processStopped, this, &ProfileEditor::taskStopped);
    connect(mlTask, &MultiTaskP::processEnd, this, &ProfileEditor::taskEnd);

    connect(ui->actionSettings, &QAction::triggered, this, &ProfileEditor::launchSettingAction);

    //Window
    connect(ui->actionToolBar, &QAction::triggered, ui->mainToolBar, &QWidget::setVisible);
    connect(ui->actionProcess, &QAction::triggered, ui->processDockWidget, &QWidget::setVisible);
//    connect(ui->actionEditor, &QAction::triggered, ui->editorDockWidget, &QWidget::setVisible);
    connect(ui->actionFileVariant, &QAction::triggered, ui->localVariantDockWidget, &QWidget::setVisible);
    connect(ui->actionAppVariant, &QAction::triggered, ui->globalVariantDockWidget, &QWidget::setVisible);
    connect(ui->actionRunSetting, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setVisible);
    connect(ui->actionRunConsole, &QAction::triggered, ui->consoleDockWidget, &BaseDockWidget::setVisible);

    connect(ui->processDockWidget, &QDockWidget::visibilityChanged, ui->actionProcess, &QAction::setChecked);
    connect(ui->localVariantDockWidget, &QDockWidget::visibilityChanged, ui->actionFileVariant, &QAction::setChecked);
    connect(ui->globalVariantDockWidget, &QDockWidget::visibilityChanged, ui->actionAppVariant, &QAction::setChecked);
//    connect(ui->editorDockWidget, &QDockWidget::visibilityChanged, ui->actionEditor, &QAction::setChecked);
    connect(ui->mainToolBar, &QToolBar::visibilityChanged, ui->actionToolBar, &QAction::setChecked);
    connect(ui->setTestDockWidget, &BaseDockWidget::visibilityChanged, ui->actionRunSetting, &QAction::setChecked);
    connect(ui->consoleDockWidget, &BaseDockWidget::visibilityChanged, ui->actionRunConsole, &QAction::setChecked);

    connect(ui->actionAutohide, &QAction::triggered, ui->processDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->consoleDockWidget, &BaseDockWidget::setAutohide);
//    connect(ui->actionAutohide, &QAction::triggered, ui->editorDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->globalVariantDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->localVariantDockWidget, &BaseDockWidget::setAutohide);

    //Infomation
    connect(ui->actionAbout, &QAction::triggered, this, &ProfileEditor::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    //end-----------------------------------------------------------------------------------------------

//    connect(editop, &EditOperator::ui_selectindexUpdate, this, &ProfileEditor::itemChangedAction);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &ProfileEditor::itemChangedAction);

//    connect(ui->graphicsView, &GraphicArea::selectChangedAction, this, &ProfileEditor::itemChangedAction);

    //Title * flag
//    connect(editop, &EditOperator::edited, this, &ProfileEditor::onFileEdited);
    connect(editop->getUndostack(), &QUndoStack::canUndoChanged, this, &ProfileEditor::onFileEdited);
//    connect(editop->getUndostack(), &QUndoStack::canRedoChanged, this, &ProfileEditor::onFileEdited);

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
                                                    QDir::currentPath()).toString(), tr("Profile (*.xml *.apro)"));

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
                                     tr("APRO Files (*.apro)"));

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

//    ui->runTreeWidget->updateIndex(updop);
    ui->innerStackedWidget->updateIndex(updop);
    ui->flowTableWidget->updateIndex(updop);
    ui->editorTab->updateIndex(updop);
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

//    ui->runTreeWidget->updateIndex(updop);
    ui->flowTableWidget->updateIndex(updop);
    ui->editorTab->updateIndex(updop);
    ui->innerStackedWidget->updateIndex(updop);
}

void ProfileEditor::addAction()
{
    editop->addAction();
    emit editop->ui_funcindexUpdate(editop->getCacheSize() - 1, -1, EditOperator::ADD, EditOperator::MAINEDITOR);

}

void ProfileEditor::deleteAction()
{
    if(dataindexpos <= 1) return;
    editop->deleteAction(dataindexpos);
    emit editop->ui_funcindexUpdate(dataindexpos, -1, EditOperator::DELETE, EditOperator::MAINEDITOR);

    dataindexpos--;
}

void ProfileEditor::cutAction()
{
    if(dataindexpos <= 1) return;
    editop->cutAction(dataindexpos);
    emit editop->ui_funcindexUpdate(dataindexpos, -1, EditOperator::DELETE, EditOperator::MAINEDITOR);

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
    emit editop->ui_funcindexUpdate(dataindexpos, -1, EditOperator::INSERT, EditOperator::MAINEDITOR);
}

void ProfileEditor::upAction()
{
    if(dataindexpos <= 2) return;
    editop->swapAction(dataindexpos, dataindexpos - 1);
    emit editop->ui_funcindexUpdate(dataindexpos - 1, dataindexpos, EditOperator::SWAP, EditOperator::MAINEDITOR);
}

void ProfileEditor::downAction()
{
    if(dataindexpos <= 0) return;
    editop->swapAction(dataindexpos, dataindexpos + 1);
    emit editop->ui_funcindexUpdate(dataindexpos + 1, dataindexpos, EditOperator::SWAP, EditOperator::MAINEDITOR);
}

void ProfileEditor::launchSettingAction()
{
    //setup settingdialog
    settingdialog->move(this->geometry().center() - settingdialog->rect().center());
    settingdialog->show();
}

//QSS_THEME
void ProfileEditor::themeChangeAction()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("pe_general");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
    settings.endGroup();

    if(stylecolor != "Default"){
#ifdef QT_DEBUG
        QFile file(QString("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/%1.qss").arg(stylecolor));
//        QFile file(QString("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/Material_Dark.qss").arg(stylecolor));

#else
        QFile file(QString(":/themes/%1.qss").arg(stylecolor));
#endif
        if(file.open( QFile::ReadOnly | QFile::Text )){
            QString data(QLatin1String(file.readAll()));
            this->setStyleSheet(data);
            settingdialog->setStyleSheet(data);
            ui->searchComboBox->setStyleSheet(data);
            ui->extrafuncComboBox->setStyleSheet(data);
            ui->searchInputComboBox->setStyleSheet(data);
            ui->fileTableWidget->setStyleSheet(data);
        }
    }
}


void ProfileEditor::overWriteSaveAction()
{
    if(editop->getLoadfile().contains(".autosave")) {
        QString fileName =
                fdialog->getSaveFileName(this,\
                                         tr("Save Edit file"),\
                                         QDir::currentPath(),\
                                         tr("APRO Files (*.apro)"));

        editop->saveAction(fileName);
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
                                     tr("XML Files (*.xml)"));
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

    setWindowTitle(newtitle + tr(" - ProfileEditor"));

}

void ProfileEditor::onFileEdited(bool edited)
{
    QString title = this->windowTitle();
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
    Q_UNUSED(before); Q_UNUSED(function); Q_UNUSED(sendfrom);

    //update show
    qDebug() << "itemChangedAction::treerowpos" << after;
    emit statusLabelChanged(tr("Process selected: No. ") + QString::number((after > 0)? after : 1));

    //update row position
    dataindexpos = after;

}

void ProfileEditor::about()
{
    //setup settingdialog
    AboutPE *ab = new AboutPE;
    ab->setStyleSheet(this->styleSheet());
    ab->move(this->geometry().center() - ab->rect().center());
    ab->show();

}

void ProfileEditor::taskStarted(QString objectname, int runfrom)
{
    qDebug() << "profileeditor::taskstarted";
    Q_UNUSED(objectname); Q_UNUSED(runfrom);
    setRunButtonState(false, true, true);
}

void ProfileEditor::taskPaused(QString objectname)
{
    qDebug() << "profileeditor::taskpaused";
    Q_UNUSED(objectname);
    setRunButtonState(true, false, true);
}

// deleting target
void ProfileEditor::taskStopped(QString objectname)
{
    qDebug() << "profileeditor::taskstopped";
    Q_UNUSED(objectname);
    setRunButtonState(true, false, false);
}

void ProfileEditor::taskEnd(QString objectname, int runfrom)
{
    qDebug() << "profileeditor::taskend";
    Q_UNUSED(runfrom);
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


    qDebug() << "profileeditor::run triggered";

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

//    ui->runTreeWidget->reloadAction();
//    ui->graphicsView->reloadAction();
    ui->innerStackedWidget->reloadAction();
    ui->flowTableWidget->reloadAction();
    ui->variantTableWidget->reloadAction();

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
