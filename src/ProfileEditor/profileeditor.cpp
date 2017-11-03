#include "profileeditor.h"
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
//    setAttribute(Qt::WA_DeleteOnClose);
    settingdialog = new SettingDialog();

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
//    sfunction = new PESharedFunction();
//    binder = new FileOperationSignalBinder(sfunction, this);
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
//    ui->variantTableWidget->setSharedFunction(sfunction);

//    ui->runTreeWidget->setSharedFunction(sfunction);

    //provide signal and slot
//    ui->runTreeWidget->setWidgetsSignalBinder(binder);
//    ui->graphicsView->setWidgetsSignalBinder(binder);

    ui->console->setRunTaskSignalBinder(rbinder);
    ui->consolemessage->setRunTaskSignalBinder(rbinder);

    //function signal bind
    connect(ui->rangeLineEdit, &QLineEdit::textChanged, rbinder, &RunTaskSignalBinder::updateRange);

    //update window title
    connect(editop, &EditOperator::loadfileChanged, this, &ProfileEditor::onTitleChanged);
//    connect(sfunction, SIGNAL(fileNameChanged(QString,QString)), this, SLOT(onWindowTitleChanged(QString, QString)));
    //update temp run file
//    connect(sfunction, &PESharedFunction::fileNameChanged, rbinder, &RunTaskSignalBinder::updateEditFile);

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
//    connect(ui->editorTab, SIGNAL(currentChanged(int)), this, SLOT(editorTabChanged(int)));

    //connect editor function (not need)
//    connectEdit();

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
    editop->openAction(loadfile);
    setLoadfile(loadfile);
    resetUi();
}

ProfileEditor::~ProfileEditor()
{
    //save window state
    QSettings settings( "./psettings.ini", QSettings::IniFormat );
    settings.setValue( "main/geometry", saveGeometry() );
    settings.setValue( "main/windowState", saveState() );

    delete ui;
    delete settingdialog;
//    delete binder;
    delete editop;
    delete rbinder;
//    delete sfunction;
}

void ProfileEditor::newfileAction()
{
//    sfunction->generateNewFile();
    editop->newAction();
    resetUi();
}

void ProfileEditor::openAction()
{
//    sfunction->openEditAction(this);
    QString fileName =
            QFileDialog::getOpenFileName(this,\
                                         tr("Open file"),\
                                         QDir::currentPath(),\
                                         tr("Profile (*.xml *.apro)"));
    if(fileName != ""){
        editop->openAction(fileName);
        resetUi();
    }
}

void ProfileEditor::saveAction()
{
    // copy only in save action
//    sfunction->saveEditAction(this);
    QString fileName =
            QFileDialog::getSaveFileName(this,\
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
//    ui->runTreeWidget->addAction();
//    binder->addItem();
    editop->addAction();

    emit editop->ui_selectindexUpdate(editop->getCacheSize() - 1, EditOperator::MAINEDITOR);
    emit editop->ui_funcindexUpdate(editop->getCacheSize() - 1, -1, EditOperator::ADD, EditOperator::MAINEDITOR);

}

//void ProfileEditor::editAction()
//{
//    //index 1 is local function.
//    qDebug() << "editor::editaction";
//    QList<QStringList> list;
//    if(treerowpos == 0){
////        createList(0, &list);
//    }
//    if(treerowpos > 1){
//        //temp list
////        createList(5, &list);
//    }
//    //TODO:
////    ui->runTreeWidget->editAction(treerowpos, &list);
////    binder->editWrite(treerowpos, &list);

//    //TODO:
////    QString data = windowTitle().split(" ").at(0);
////    setWindowTitle(data + "*");
//}

//void ProfileEditor::editAction(int)
//{
//    editAction();
//}

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

void ProfileEditor::themeChangeAction()
{
    QSettings settings( "./psettings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("BASICSETTING");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
    settings.endGroup();

    if(stylecolor == "Light"){
#ifdef QT_DEBUG
        QFile file("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/Light.qss");
#else
        QFile file(":/themes/Light.qss");
#endif
        if(file.open( QFile::ReadOnly | QFile::Text )){
            QString data(QLatin1String(file.readAll()));
//            data = data.replace(QRegularExpression("(\r\n|\r|\n)"), "");
            this->setStyleSheet(data);
        }
    }

    if(stylecolor == "Dark"){
#ifdef QT_DEBUG
        QFile file("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/Dark.qss");
#else
        QFile file(":/themes/Dark.qss");
#endif
        if(file.open( QFile::ReadOnly | QFile::Text )){
            QString data(QLatin1String(file.readAll()));
            this->setStyleSheet(data);
        }
    }
}


void ProfileEditor::overWriteSaveAction()
{
//     sfunction->saveEditOverWriteAction(this);
    if(loadfile.contains(".autosave")) {
        QString fileName =
                QFileDialog::getSaveFileName(this,\
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
            QFileDialog::getSaveFileName(this,\
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
    ui->localVariantComboBox->setProfileFileName(newload);

    //update filepath
    setLoadfile(newload);
    rbinder->updateEditFile(newload);
}

//void ProfileEditor::onWindowTitleChanged(QString newtitle, QString newtemp)
//{
//    ///HARD CODING
//    if(newtitle != "New_File"){
//        QFileInfo info(newtitle);
//        newtitle = info.fileName();
//    }

//    setWindowTitle(newtitle + tr(" - ProfileEditor"));

//    //set new update file (search, script)
//    ui->localVariantComboBox->setProfileFileName(newtemp);
////    ui->varComboBox->setProfileFileName(newtemp);
//}

//void ProfileEditor::itemChangedAction()
//{
//    qDebug() << "itemChangedAction::called";
//    //create list item
//    QList<QStringList> item;

//    //TODO:
//    if(!ui->runTreeWidget->currentItem()->parent()){

//        treerowpos = ui->runTreeWidget->fixedCurrentRow();

//        qDebug() << "itemChangedAction::treerowpos" << treerowpos;

//        //set new string
//        binder->readItem(treerowpos, &item);
//        setSettingList(&item);

////        ui->runTreeWidget->expandItem(ui->runTreeWidget->currentItem());

//        int index = (treerowpos > 0) ? treerowpos - 1 : 0;
//        emit rowPosChanged("Selected: " + QString::number(index));
//    }
//    //TODO:do collapse
//}

void ProfileEditor::itemChangedAction(int index)
{
    //create list item
//    QList<QStringList> item;
//    treerowpos = (index > 0)? index + 1 : 0;

    //set new string
//    binder->readItem(treerowpos, &item);
//    setSettingList(&item);
//    if(index > 0){
//        ui->innerStackedWidget->setCurrentIndex(1);
//    }else{
//        ui->innerStackedWidget->setCurrentIndex(0);
//    }


    qDebug() << "itemChangedAction::treerowpos" << index;
    emit rowPosChanged("Selected: " + QString::number((index > 0)? index : 1));
}

//this function detects value "-1". it shows "xml local data"
//void ProfileEditor::editorTabChanged(int index)
//{
//    qDebug() << "runtypeComboBoxChanged::index" << index;

//    //set new index
//    if(index > -1) ui->innerStackedWidget->setCurrentIndex(1);

////    //disconnect
////    disconnectEdit();

////    switch(index){
////    case 1:
////        ui->searchComboBox->reloadComboBoxItem();
////        ui->localVariantComboBox->reloadComboBoxItem();
////        break;
////    case 2:
////        ui->extrafuncComboBox->reloadComboBoxItem();
//////        ui->varComboBox->reloadComboBoxItem();
////        break;
////    case 3:
////        ui->profileComboBox->reloadComboBoxItem();
////        break;
////    default:
////        break;
////    }

//////    if(index > -1) this->editAction();

//////    ui->graphicsView->reloadAction();

////    //re-connect
////    connectEdit();
//}

void ProfileEditor::about()
{
    QMessageBox::about(this, tr("About ProfileEditor")
                       , tr("AutoBatchRunner - ProfileEditor ver.beta\r\n\r\n"
                            "This is an editor of AutoBatchRunner.\r\n"
                            "ProfileEditor can create execution list of other projects.\r\n\r\n"
                            "Currently, this program runs only windows systems.\r\n"
                            "These programs licensed under GNU LGPL version 3 for now.\r\n\r\n"
                            "Made by mr_elphis in 2017/11/03"));
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

    if(!editop->isEdited()){
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

//void ProfileEditor::connectEdit()
//{
//    //type=info
//    connect(ui->nameLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    connect(ui->verLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    connect(ui->authorLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    connect(ui->descTextEdit, SIGNAL(textChanged()), this, SLOT(editAction()));

//    //common in innerstack
//    connect(ui->autoOnlyCheckBox, SIGNAL(clicked()), this, SLOT(editAction()));
//    connect(ui->autoOnlyCheckBox_2, SIGNAL(clicked()), this, SLOT(editAction()));
//    connect(ui->autoOnlyCheckBox_3, SIGNAL(clicked()), this, SLOT(editAction()));
//    connect(ui->autoOnlyCheckBox_4, SIGNAL(clicked()), this, SLOT(editAction()));

//    //type=normal
//    connect(ui->timeoutCheckBox, SIGNAL(clicked()), this, SLOT(editAction()));
//    connect(ui->timeoutSpinBox, SIGNAL(valueChanged(int)), this, SLOT(editAction()));
//    connect(ui->cmdTableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(editAction()));

//    //type=search
//    connect(ui->searchComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//    connect(ui->separatorLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    connect(ui->localVariantComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//    connect(ui->outputLineEdit, SIGNAL(textChanged(QString)), this, SLOT(editAction()));

//    //type=extrafunc
//    connect(ui->extrafuncComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
////    connect(ui->varComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//    connect(ui->extrafuncTableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(editAction()));

//    //type=other
//    connect(ui->profileComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//}

//void ProfileEditor::disconnectEdit()
//{
//    //type=info
//    disconnect(ui->nameLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    disconnect(ui->verLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    disconnect(ui->authorLineEdit, SIGNAL(editingFinished()), this, SLOT(editAction()));
//    disconnect(ui->descTextEdit, SIGNAL(textChanged()), this, SLOT(editAction()));

//    //common in innerstack
//    disconnect(ui->autoOnlyCheckBox, SIGNAL(clicked()), this, SLOT(editAction()));
//    disconnect(ui->autoOnlyCheckBox_2, SIGNAL(clicked()), this, SLOT(editAction()));
//    disconnect(ui->autoOnlyCheckBox_3, SIGNAL(clicked()), this, SLOT(editAction()));
//    disconnect(ui->autoOnlyCheckBox_4, SIGNAL(clicked()), this, SLOT(editAction()));

//    //type=normal
//    disconnect(ui->timeoutCheckBox, SIGNAL(clicked()), this, SLOT(editAction()));
//    disconnect(ui->timeoutSpinBox, SIGNAL(valueChanged(int)), this, SLOT(editAction()));
//    disconnect(ui->cmdTableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(editAction()));

//    //type=search
//    disconnect(ui->searchComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//    disconnect(ui->separatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(editAction()));
//    disconnect(ui->localVariantComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//    disconnect(ui->outputLineEdit, SIGNAL(textChanged(QString)), this, SLOT(editAction()));

//    //type=extrafunc
//    disconnect(ui->extrafuncComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
////    disconnect(ui->varComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//    disconnect(ui->extrafuncTableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(editAction()));

//    //type=other
//    disconnect(ui->profileComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAction(int)));
//}

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


//void ProfileEditor::setSettingList(QList<QStringList> *itemlist)
//{
//    //edit disconnect
//    disconnectEdit();

//    if(itemlist->empty()) return;

//    int runtype = sfunction->setInformationType(itemlist->at(0).at(1));

//    ui->innerStackedWidget->moveStacked(runtype);
//    ui->editorTab->moveTabFromXml(runtype);

//    qDebug() << "setSettingList :: runtype : " << runtype;

//    switch(runtype){
//    case 0: /*setInfoList(itemlist, 1);*/
//        ui->innerStackedWidget->setInfoDataList(itemlist, 1);
//        break;
//    case 1: /*setNormalList(itemlist, 1);*/
////        ui->autoOnlyCheckBox->setChecked(VariantConverter::stringToBool(itemlist->at(0).at(3)));
//        ui->editorTab->setNormalDataList(itemlist, 0);
//        ui->editorTab->setCurrentIndex(0);
//        break;
//    case 2: /*setSearchList(itemlist, 1);*/
////        ui->autoOnlyCheckBox_2->setChecked(VariantConverter::stringToBool(itemlist->at(0).at(3)));
//        ui->editorTab->setSearchDataList(itemlist, 0);
//        ui->editorTab->setCurrentIndex(1);
//        break;
//    case 3: /*setScriptList(itemlist, 1);*/
////        ui->autoOnlyCheckBox_3->setChecked(VariantConverter::stringToBool(itemlist->at(0).at(3)));
//        ui->editorTab->setScriptDataList(itemlist, 0);
//        ui->editorTab->setCurrentIndex(2);
//        break;
//    case 4: /*ui->profileComboBox->setCurrentText(itemlist->at(1).at(1));*/
////        ui->autoOnlyCheckBox_4->setChecked(VariantConverter::stringToBool(itemlist->at(0).at(3)));
//        ui->editorTab->setOtherDataList(itemlist, 0);
//        ui->editorTab->setCurrentIndex(3);
//        break;
//    case 5: /*setSettingTempList(itemlist);*/
//        //tab index is determined inner of this function.
////        ui->editorTab->setTempDataList(itemlist, sfunction);
//        break;
//    default:
//        break;
//    }

//    connectEdit();
//}

///DEPENDS_XML
//void ProfileEditor::setSettingTempList(QList<QStringList> *list)
//{
//    if(list->empty()) return;

//    //no file showing module
//    int cmdskip = VariantConverter::stringToInt(list->at(sfunction->firstPosNormal() + 1).at(1));
////    int scrskip = VariantConverter::stringToInt(list->at(8 + cmdskip).at(1));
//    //set autoonly checkbox
//    //TODO: DO NOT ALL DATA CHANGED (lest _2, _3, _4)
////    ui->autoOnlyCheckBox->setChecked(VariantConverter::stringToBool(list->at(0).at(3)));

//    //set widget selection
////    ui->innerStackedWidget->setCurrentIndex(1);
//    //set runtype combobox selection from innerstackedwidget
////    ui->runTypeComboBox->setCurrentIndex(VariantConverter::stringToInt(list->at(1).at(1)));
//    ui->editorTab->setCurrentIndex(VariantConverter::stringToInt(list->at(1).at(1)));

//    //load type=normal
//    ui->editorTab->setNormalDataList(list, sfunction->firstPosNormal());

//    //load type=search
//    ui->editorTab->setSearchDataList(list, sfunction->firstPosSearch());

//    //load type=script
//    ui->editorTab->setScriptDataList(list, sfunction->firstPosScript() + cmdskip);

//    //load type=other
//    ui->editorTab->setOtherDataList(list, sfunction->firstPosOther());

//    //no file showing module
//}

/**
 * @brief ProfileEditor::createList
 * @param itemid
 * @param type
 * @param newlist
 *
 * @details
 * int type
 * 0:info
 * 1:normal
 * 2:search
 * 3:script
 * 4:other
 */
//void ProfileEditor::createList(int type, QList<QStringList> *newlist)
//{
//    switch(type){
//    case 0: getInfoList(newlist);  break;
//    case 1: getNormalList(newlist);break;
//    case 2: getSearchList(newlist);break;
//    case 3: getScriptList(newlist);break;
//    case 4: getOtherList(newlist); break;
//    case 5: createTempList(newlist);     break;
//    default:                             break;
//    }
//}

///DEPENDS_XML
//void ProfileEditor::createTempList(QList<QStringList> *newlist)
//{
//    // basic infomation
//    newlist->append((QStringList() << "type" << "temp" << "only"
//                     << VariantConverter::boolToString(ui->editorTab->getCurrentIndexOnlyChecked())));
//    newlist->append((QStringList() << "istack" << QString::number(ui->editorTab->currentIndex())));

//    // "type" << "search";
//    getSearchList(newlist);

//    // "type" << "other";
//    getOtherList(newlist);

//    // "type" << "normal";
//    getNormalList(newlist);

//    // "type" << "script";
//    getScriptList(newlist);

//}

/////DEPENDS_XML
//void ProfileEditor::getInfoList(QList<QStringList> *newlist)
//{
//    QStringList list;
//    ui->innerStackedWidget->getInfoDataList(&list);
//    sfunction->createInfoList(newlist, &list);
//}

/////DEPENDS_XML
//void ProfileEditor::getNormalList(QList<QStringList> *newlist)
//{
//    QStringList list;
//    ui->editorTab->getNormalDataList(&list);
//    sfunction->createNormalList(newlist, &list);
//}

/////DEPENDS_XML
//void ProfileEditor::getSearchList(QList<QStringList> *newlist)
//{
//    QStringList list;
//    ui->editorTab->getSearchDataList(&list);
//    sfunction->createSearchList(newlist, &list);
//}

/////DEPENDS_XML
//void ProfileEditor::getScriptList(QList<QStringList> *newlist)
//{
//    QStringList list;
//    ui->editorTab->getScriptDataList(&list);
//    sfunction->createScriptList(newlist, &list);
//}

/////DEPENDS_XML
//void ProfileEditor::getOtherList(QList<QStringList> *newlist)
//{
//    QStringList list;
//    ui->editorTab->getOtherDataList(&list);
//    sfunction->createOtherList(newlist, &list);
//}

void ProfileEditor::setLoadfile(const QString &value)
{
    loadfile = value;
}

//when opening new file
void ProfileEditor::resetUi()
{
    //set current info ui
    ui->innerStackedWidget->setCurrentIndex(0);

    //temp disconnect slot
//    disconnectEdit();

    //set firststacked to empty
    ui->innerStackedWidget->clearInfoDataListForm();

    //reload all combobox
//    ui->searchComboBox->reloadComboBoxItem();
//    ui->extrafuncComboBox->reloadComboBoxItem();
    ui->localVariantComboBox->reloadComboBoxItem();
//  ui->varComboBox->reloadComboBoxItem();
//    ui->profileComboBox->reloadComboBoxItem();

    //reload file
    ui->runTreeWidget->reloadAction();
//    ui->graphicsView->reloadAction();
    ui->variantTableWidget->reloadAction();
    ui->flowTableWidget->reloadAction();

    //connect slot
//    connectEdit();

    //reset tree row position
    rowpos = 0;

    //load first item
//    QList<QStringList> item;
//    sfunction->readItem(treerowpos, &item);
//    setSettingList(&item);
}
