﻿#include "profileeditor.h"
#include "ui_profileeditor.h"

#include <QProgressBar>

ProfileEditor::ProfileEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileEditor)
{
    ui->setupUi(this);

    //dock position
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    //set docktab
    tabifyDockWidget(ui->localVariantDockWidget, ui->globalVariantDockWidget);
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::TabPosition::East);
    ui->localVariantDockWidget->show();

    //Window data restore
    QSettings settings( "./psettings.ini", QSettings::IniFormat );
    QVariant v = settings.value( "main/geometry" );
    if (v.type() != QVariant::Invalid){
        // load window settings on MainWindow
        restoreGeometry( settings.value( "main/geometry" ).toByteArray() );
        restoreState( settings.value( "main/windowState" ).toByteArray() );
    }

    //set Attrs
    settingdialog = new SettingDialog();
    fdialog = new QFileDialog();

    //theme settings
    themeChangeAction();

    //set dock autohide TODO: not saved menu
    ui->processDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->setTestDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->runTestDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->editorDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->globalVariantDockWidget->setAutohide(ui->actionAutohide->isChecked());
    ui->localVariantDockWidget->setAutohide(ui->actionAutohide->isChecked());

    //init sharedfunction
    editop = new EditOperator(this);
    rbinder = new RunTaskSignalBinder(this);

    //connection set
    ui->editorTab->setConnection();

    //provide operator
    ui->runTreeWidget->setEditOperator(editop);
//    ui->graphicsView->setEditOperator(editop);
    ui->flowTableWidget->setEditOperator(editop);
    ui->variantTableWidget->setEditOperator(editop);
    ui->editorTab->setEditOperator(editop);
    ui->innerStackedWidget->setEditOperator(editop);

    //test
//    ui->graphicsView->hide();

    //update tree position
    connect(editop, &EditOperator::ui_selectindexUpdate, this, &ProfileEditor::setTreerowpos_select);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &ProfileEditor::setTreerowpos_update);
    //provide function object

    ui->console->setRunTaskSignalBinder(rbinder);
    ui->consolemessage->setRunTaskSignalBinder(rbinder);

    //function signal bind
    connect(ui->rangeLineEdit, &QLineEdit::textChanged, rbinder, &RunTaskSignalBinder::updateRange);

    //update window title
    connect(editop, &EditOperator::loadfileChanged, this, &ProfileEditor::onTitleChanged);

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
//    connect(ui->actionAddItem, SIGNAL(triggered()), binder, SLOT(addItem()));
    connect(ui->actionUndo, &QAction::triggered, this, &ProfileEditor::undoAction);
    connect(ui->actionRedo, &QAction::triggered, this, &ProfileEditor::redoAction);
    connect(ui->actionAddItem, &QAction::triggered, this, &ProfileEditor::addAction);
    connect(ui->actionDeleteItem, SIGNAL(triggered()), this, SLOT(deleteAction()));
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
    connect(ui->actionRun, &QAction::triggered, rbinder, &RunTaskSignalBinder::start);

    connect(ui->actionPause, &QAction::triggered, rbinder, &RunTaskSignalBinder::pause);
    connect(ui->actionPause, &QAction::triggered, ui->actionPause, &QAction::setDisabled);
    connect(ui->actionPause, &QAction::triggered, ui->pauseToolButton, &QToolButton::setDisabled);

    connect(ui->actionStop, &QAction::triggered, rbinder, &RunTaskSignalBinder::stop);

    connect(rbinder, &RunTaskSignalBinder::processStarted_action, ui->actionRun, &QAction::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processStarted_action, ui->actionPause, &QAction::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processStarted_action, ui->actionStop, &QAction::setEnabled);

    connect(rbinder, &RunTaskSignalBinder::processPaused_action, ui->actionRun, &QAction::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processPaused_action, ui->actionPause, &QAction::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processPaused_action, ui->actionStop, &QAction::setEnabled);

    connect(rbinder, &RunTaskSignalBinder::processStopped_action, ui->actionRun, &QAction::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processStopped_action, ui->actionStop, &QAction::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processStopped_action, ui->actionPause, &QAction::setDisabled);

    connect(rbinder, &RunTaskSignalBinder::processEnd_action, ui->actionRun, &QAction::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processEnd_action, ui->actionStop, &QAction::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processEnd_action, ui->actionPause, &QAction::setDisabled);

    connect(ui->runToolButton, &QToolButton::clicked, rbinder, &RunTaskSignalBinder::start);

    connect(ui->pauseToolButton, &QToolButton::clicked, rbinder, &RunTaskSignalBinder::pause);
    connect(ui->pauseToolButton, &QToolButton::clicked, ui->pauseToolButton, &QToolButton::setDisabled);
    connect(ui->pauseToolButton, &QToolButton::clicked, ui->actionPause, &QAction::setDisabled);

    connect(ui->stopToolButton, &QToolButton::clicked, rbinder, &RunTaskSignalBinder::stop);

    connect(rbinder, &RunTaskSignalBinder::processStarted_action, ui->runToolButton, &QToolButton::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processStarted_action, ui->pauseToolButton, &QToolButton::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processStarted_action, ui->stopToolButton, &QToolButton::setEnabled);

    connect(rbinder, &RunTaskSignalBinder::processPaused_action, ui->runToolButton, &QToolButton::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processPaused_action, ui->pauseToolButton, &QToolButton::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processPaused_action, ui->stopToolButton, &QToolButton::setEnabled);

    connect(rbinder, &RunTaskSignalBinder::processStopped_action, ui->runToolButton, &QToolButton::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processStopped_action, ui->stopToolButton, &QToolButton::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processStopped_action, ui->pauseToolButton, &QToolButton::setDisabled);

    connect(rbinder, &RunTaskSignalBinder::processEnd_action, ui->runToolButton, &QToolButton::setEnabled);
    connect(rbinder, &RunTaskSignalBinder::processEnd_action, ui->stopToolButton, &QToolButton::setDisabled);
    connect(rbinder, &RunTaskSignalBinder::processEnd_action, ui->pauseToolButton, &QToolButton::setDisabled);

    connect(ui->actionSettings, &QAction::triggered, this, &ProfileEditor::launchSettingAction);

    //Window
    connect(ui->actionProcessTree, &QAction::triggered, ui->processDockWidget, &QWidget::setVisible);
    connect(ui->actionLocalVariant, &QAction::triggered, ui->localVariantDockWidget, &QWidget::setVisible);
    connect(ui->actionGlobalVariant, &QAction::triggered, ui->globalVariantDockWidget, &QWidget::setVisible);
    connect(ui->actionEditor, &QAction::triggered, ui->editorDockWidget, &QWidget::setVisible);
    connect(ui->actionToolBar, &QAction::triggered, ui->mainToolBar, &QWidget::setVisible);
    connect(ui->processDockWidget, &QDockWidget::visibilityChanged, ui->actionProcessTree, &QAction::setChecked);
    connect(ui->localVariantDockWidget, &QDockWidget::visibilityChanged, ui->actionLocalVariant, &QAction::setChecked);
    connect(ui->globalVariantDockWidget, &QDockWidget::visibilityChanged, ui->actionGlobalVariant, &QAction::setChecked);
    connect(ui->editorDockWidget, &QDockWidget::visibilityChanged, ui->actionEditor, &QAction::setChecked);
    connect(ui->mainToolBar, &QToolBar::visibilityChanged, ui->actionToolBar, &QAction::setChecked);

    connect(ui->actionAutohide, &QAction::triggered, ui->processDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->setTestDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->runTestDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->editorDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->globalVariantDockWidget, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->localVariantDockWidget, &BaseDockWidget::setAutohide);

    //Infomation
    connect(ui->actionAbout, &QAction::triggered, this, &ProfileEditor::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    //end-----------------------------------------------------------------------------------------------

    connect(editop, &EditOperator::ui_selectindexUpdate, this, &ProfileEditor::itemChangedAction);
//    connect(ui->graphicsView, &GraphicArea::selectChangedAction, this, &ProfileEditor::itemChangedAction);

    //Title * flag
//    connect(editop, &EditOperator::edited, this, &ProfileEditor::onFileEdited);
    connect(editop->getUndostack(), &QUndoStack::canUndoChanged, this, &ProfileEditor::onFileEdited);

    initStatusBar();

    //set new file
    newfileAction();
}

//with loading constructor
ProfileEditor::ProfileEditor(QString loadfile, QWidget *parent)
    : ProfileEditor(parent)
{
//    sfunction->loadFile(loadfile);
//    ui->runTreeWidget->reloadAction();
//    ui->graphicsView->reloadAction();
//    ui->variantTableWidget->reloadAction();
//    preResetUi();
    editop->openAction(loadfile);
//    setLoadfile(loadfile); /*int title changed*/
    postResetUi();
}

ProfileEditor::~ProfileEditor()
{
    //save window state
    QSettings settings( "./psettings.ini", QSettings::IniFormat );
    settings.setValue( "main/geometry", saveGeometry() );
    settings.setValue( "main/windowState", saveState() );

    delete ui;
    delete settingdialog;
    delete fdialog;
    delete editop;
    delete rbinder;
}

void ProfileEditor::newfileAction()
{
    if(lastedited){
        // ドキュメントが変更されている場合の警告
        QMessageBox::StandardButton res
                = QMessageBox::question(this,\
                                        tr("Alert"),\
                                        tr("File was edited.\nDo you want to save this file ?"),\
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );


        if(res == QMessageBox::Cancel) return;
        if(res == QMessageBox::Yes){
            // save and generate new file
            overWriteSaveAction();
        }
    }

    // not save and newfile generate
    editop->newAction();
    postResetUi();

}

void ProfileEditor::openAction()
{
    if(lastedited){
        // Warning if document is edited by user.
        QMessageBox::StandardButton res
                = QMessageBox::question(this,\
                                        tr("Alert"),\
                                        tr("File was edited.\nDo you want to save this file ?"),\
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );



        if(res == QMessageBox::Cancel) return;


        if(res == QMessageBox::Yes){
            // save file
            overWriteSaveAction();
        }
    }

    // open file
    QString fileName =
            fdialog->getOpenFileName(this,\
                                     tr("Open file"),\
                                     QDir::currentPath(),\
                                     tr("Profile (*.xml *.apro)"));
    if(fileName != ""){
//          preResetUi();
        editop->openAction(fileName);
        postResetUi();
    }

}

void ProfileEditor::saveAction()
{
    // copy only in save action
    QString fileName =
            fdialog->getSaveFileName(this,\
                                     tr("Save Edit file"),\
                                     QDir::currentPath(),\
                                     tr("APRO Files (*.apro)"));

    if(fileName != ""){
        editop->saveAction(fileName);
    }
}

void ProfileEditor::undoAction()
{
    if(editop->getUndostack()->canUndo()){
        QUndoStack *stack = editop->getUndostack();
        stack->undo();

        //TODO: not efficient
        ui->runTreeWidget->reloadAction();
        ui->variantTableWidget->reloadAction();
        ui->flowTableWidget->reloadAction();

    }
}

void ProfileEditor::redoAction()
{
    if(editop->getUndostack()->canRedo()){
        editop->getUndostack()->redo();
        //TODO: not efficient
        ui->runTreeWidget->reloadAction();
        ui->variantTableWidget->reloadAction();
        ui->flowTableWidget->reloadAction();
    }
}

void ProfileEditor::addAction()
{
    editop->addAction();

    emit editop->ui_selectindexUpdate(editop->getCacheSize() - 1, EditOperator::MAINEDITOR);
    emit editop->ui_funcindexUpdate(editop->getCacheSize() - 1, -1, EditOperator::ADD, EditOperator::MAINEDITOR);

}

void ProfileEditor::deleteAction()
{
    if(rowpos > 1){
    //    binder->deleteItem(treerowpos);
        editop->deleteAction(rowpos);
        //emit editop->ui_selectindexUpdate(, EditOperator::MAINEDITOR);

        emit editop->ui_selectindexUpdate(rowpos, EditOperator::MAINEDITOR);
        emit editop->ui_funcindexUpdate(rowpos, -1, EditOperator::DELETE, EditOperator::MAINEDITOR);
    }
}

void ProfileEditor::cutAction()
{
    if(rowpos > 1){
        editop->cutAction(rowpos);
        emit editop->ui_selectindexUpdate(rowpos, EditOperator::MAINEDITOR);
        emit editop->ui_funcindexUpdate(rowpos, -1, EditOperator::DELETE, EditOperator::MAINEDITOR);
    }
}

void ProfileEditor::copyAction()
{
//    ui->runTreeWidget->copyAction();
    if(rowpos > 1){
        editop->copyAction(rowpos);
    }
}

void ProfileEditor::pasteAction()
{
    if(rowpos > 1){
        editop->pasteAction(rowpos);
        emit editop->ui_selectindexUpdate(rowpos, EditOperator::MAINEDITOR);
        emit editop->ui_funcindexUpdate(rowpos, -1, EditOperator::INSERT, EditOperator::MAINEDITOR);
    }
}

void ProfileEditor::upAction()
{
//    binder->upItem(treerowpos);
    if(rowpos > 2){
        editop->swapAction(rowpos, rowpos - 1);
        emit editop->ui_selectindexUpdate(rowpos - 1, EditOperator::MAINEDITOR);
        emit editop->ui_funcindexUpdate(rowpos - 1, rowpos, EditOperator::SWAP, EditOperator::MAINEDITOR);
    }
}

void ProfileEditor::downAction()
{
//    binder->downItem(treerowpos);
    if(rowpos < (editop->getCacheSize() - 1)){
        editop->swapAction(rowpos, rowpos + 1);
        emit editop->ui_selectindexUpdate(rowpos + 1, EditOperator::MAINEDITOR);
        emit editop->ui_funcindexUpdate(rowpos + 1, rowpos, EditOperator::SWAP, EditOperator::MAINEDITOR);
    }
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
    QSettings settings( "./psettings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("BASICSETTING");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
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
//            settingdialog->setStyleSheet(data);
        }
    }
}


void ProfileEditor::overWriteSaveAction()
{
//     sfunction->saveEditOverWriteAction(this);
    if(loadfile.contains(".autosave")) {
        QString fileName =
                fdialog->getSaveFileName(this,\
                                         tr("Save Edit file"),\
                                         QDir::currentPath(),\
                                         tr("APRO Files (*.apro)"));

        editop->saveAction(fileName);
        loadfile = fileName;
    }
    editop->saveAction(loadfile);
}

void ProfileEditor::exportAction()
{
//    sfunction->exportData(this);
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

    //set new update file (search, script)
//    ui->localVariantComboBox->setProfileFileName(newload);

    //update filepath
    setLoadfile(newload);
    rbinder->updateEditFile(newload);
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
        if(title.at(0) == "*"){
            setWindowTitle(title.remove(0,1));
        }
    }
}

void ProfileEditor::itemChangedAction(int index)
{
    qDebug() << "itemChangedAction::treerowpos" << index;
    emit rowPosChanged("Selected: " + QString::number((index > 0)? index : 1));
}

void ProfileEditor::about()
{
    QMessageBox::about(this, tr("About ProfileEditor")
                       , tr("AutoBatchRunner - ProfileEditor ver.beta\r\n\r\n"
                            "This is an editor of AutoBatchRunner.\r\n"
                            "ProfileEditor can create execution list of other projects.\r\n\r\n"
                            "Currently, this program runs only windows systems.\r\n"
                            "These programs licensed under GNU LGPL version 3 for now.\r\n\r\n"
                            "Made by mr_elphis in 2017/11/17"));
}

void ProfileEditor::setTreerowpos_select(int value, int from)
{
    if(from == EditOperator::MAINEDITOR) return;
    rowpos = value;
}

void ProfileEditor::setTreerowpos_update(int after, int before, int function, int sendfrom)
{
    Q_UNUSED(before);
    Q_UNUSED(function);
    Q_UNUSED(sendfrom);
    rowpos = after;
}

void ProfileEditor::onUndoTextChanged(QString text)
{
    ui->actionUndo->setText(tr("Undo %1").arg(text));
}

void ProfileEditor::onRedoTextChanged(QString text)
{
    ui->actionRedo->setText(tr("Redo %1").arg(text));
}

void ProfileEditor::closeEvent(QCloseEvent *event)
{
    // check file difference (0: same file generated)
//    int fileedit = sfunction->checkFileEdited();
//    qDebug() << "closeEvent::checkdiff" << fileedit;

    if(!lastedited){
        return;
    }

    // ドキュメントが変更されている場合の警告
    QMessageBox::StandardButton res
            = QMessageBox::question(this,\
                                    tr("Alert"),\
                                    tr("File was edited.\nDo you want to save this file ?"),\
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:
      // 保存できたなら、閉じてOK
      overWriteSaveAction();
      break;

    case QMessageBox::No:
      // 保存しなくて、そのまま閉じてOK
      editop->abortAction();
      break;

    case QMessageBox::Cancel:
      // キャンセルして作業に戻る
      event->ignore();

    default:
      break;
    }
}

void ProfileEditor::initStatusBar()
{
    QLabel *label = new QLabel();
    label->setAlignment(Qt::AlignLeft);
    ui->statusBar->addPermanentWidget(label, 3);
    connect(this, SIGNAL(rowPosChanged(QString)), label, SLOT(setText(QString)));

    QProgressBar *progressbar = new QProgressBar();
    progressbar->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(progressbar, 1);
    connect(rbinder, &RunTaskSignalBinder::processInitCount, progressbar, &QProgressBar::setRange);
    connect(rbinder, &RunTaskSignalBinder::processCurrent, progressbar, &QProgressBar::setValue);
}

void ProfileEditor::setLoadfile(const QString &value)
{
    loadfile = value;
}

//void ProfileEditor::preResetUi()
//{
//    //set current info ui
////    ui->innerStackedWidget->setCurrentIndex(0);

//    //temp disconnect slot
////    disconnectEdit();

//    //set firststacked to empty
////    ui->innerStackedWidget->clearInfoDataListForm();
//}

//when opening new file
void ProfileEditor::postResetUi()
{
    //reload variant combobox
//    ui->localVariantComboBox->reloadComboBoxItem();

    //reload file
    ui->runTreeWidget->reloadAction();
//    ui->graphicsView->reloadAction();
    ui->flowTableWidget->reloadAction();
    ui->variantTableWidget->reloadAction();

    //reset tree row position
    rowpos = 0;
}
