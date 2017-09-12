#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //set dockwidget occupied area
//    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
//    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //setup ui
    ui->setupUi(this);

    //Window init
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    QVariant v = settings.value( "main/geometry" );
    if (v.type() != QVariant::Invalid){
        // load window settings on MainWindow
        restoreGeometry( settings.value( "main/geometry" ).toByteArray() );
        restoreState( settings.value( "main/windowState" ).toByteArray() );
    }

    //set menu file
    connect(ui->actionExit, &QAction::triggered, qApp, &QCoreApplication::quit);

    //set dock autohide TODO: not saved menu
    ui->runDock->setAutohide(ui->actionAutohide->isChecked());
    ui->consoleDock->setAutohide(ui->actionAutohide->isChecked());

    //set menu show
    connect(ui->actionAutohide, &QAction::triggered, ui->runDock, &BaseDockWidget::setAutohide);
    connect(ui->actionAutohide, &QAction::triggered, ui->consoleDock, &BaseDockWidget::setAutohide);
    connect(ui->runDock, &BaseDockWidget::visibilityChanged, ui->actionAutohide, &QAction::setChecked);
    connect(ui->consoleDock, &BaseDockWidget::visibilityChanged, ui->actionAutohide, &QAction::setChecked);


    //init system tray
    //test function
    sysTray = new SystemTray();
    sysTray->show();

    //connect systray actions
    connect(sysTray, &SystemTray::launchMain, this, &MainWindow::showThisWindow);
    connect(sysTray, &SystemTray::launchSetting, this, &MainWindow::on_actionSettings_triggered);
    connect(sysTray, &SystemTray::launchclose, this, &MainWindow::closedFromSystemTray);

    //TODO: error occured
//    ui->comboBox->reloadComboBoxItem();
    //init combobox
//    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxItemChanged(int)));

    //select combobox first
//    if(ui->comboBox->count() > 0) comboBoxItemChanged(0);

    //init optiondialog object
    opdialog = new OptionDialog();
//    connect(opdialog, SIGNAL(accepted()), ui->comboBox, SLOT(reloadComboBoxItem()));

    //spinbox min set
//    ui->rCountMinSpinBox->setMinimum(1);
//    ui->rCountMaxSpinBox->setMinimum(1);


//    if(ui->comboBox->currentText() != "") ui->comboBox->currentIndexChanged(0);

    //connect combobox and tree widget
//    ui->profileTreeWidget->init_LoadCombobox(ui->comboBox);
}

MainWindow::~MainWindow()
{
    //save window state
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.setValue( "main/geometry", saveGeometry() );
    settings.setValue( "main/windowState", saveState() );


    //delete object
    delete opdialog;
    delete sysTray;
    delete ui;
}

//----------------------------------------------------------------
//menu action slot (docksettings)
//void MainWindow::on_actionConsole_triggered(bool checked)
//{
////    checked ? ui->consoleDock->show() : ui->consoleDock->hide();
//}

//void MainWindow::on_actionRun_triggered(bool checked)
//{
////    checked ? ui->runDock->show() : ui->runDock->hide();
//}

//void MainWindow::on_consoleDock_visibilityChanged(bool visible)
//{
//    ui->actionConsole->setChecked(visible);
//}

//void MainWindow::on_runDock_visibilityChanged(bool visible)
//{
//    ui->actionRun->setChecked(visible);
//}
//----------------------------------------------------------------

void MainWindow::on_actionSettings_triggered()
{
    if(!this->isVisible()){
        if(this->isMinimized()){
            this->showNormal();
        }else{
            this->show();
        }
    }
    opdialog->move(this->geometry().center() - opdialog->rect().center());
    opdialog->show();
}

void MainWindow::showThisWindow()
{
    if(this->isMinimized()){
        this->showNormal();
    }else{
        this->show();
    }
}

void MainWindow::closedFromSystemTray()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    bool allclose = settings.value("ALLCLOSE", true).toBool();
    settings.endGroup();
    if(allclose){
        QCoreApplication::quit();
    }else{
        sysTray->hide();
        this->showNormal();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //TODO:
    systemTraySelect() ? event->ignore() : event->accept();
}

bool MainWindow::systemTraySelect()
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    //when startup action selected, hide window.
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    bool isstartup = settings.value("ENABLED", false).toBool();
    settings.endGroup();

    if(isstartup) this->hide();

    return isstartup;
}

void MainWindow::on_runButton_clicked()
{
//    MainProcessLoader *loader = new MainProcessLoader();
//    loader->setCounterMin(ui->rCountMinSpinBox->value());
//    loader->setCounterMax(ui->rCountMaxSpinBox->value());
//    loader->setFileName(ui->comboBox->getCurrentFileName());
//    loader->setUpdateTextEdit(ui->commandLineText);
//    loader->start();
}

//void MainWindow::on_editButton_clicked()
//{
//    QString filename = ui->comboBox->getCurrentFileName();
//    if(filename != ""){
//        ProfileEditor *pe = new ProfileEditor(filename);
//        connect(pe, SIGNAL(destroyed(QObject*)), ui->profileTreeWidget, SLOT(reloadAction()));
//        pe->show();
//    }
//}

void MainWindow::comboBoxItemChanged(int index)
{
//    ui->profileTreeWidget->loadFile(ui->comboBox->getCurrentFileName(index));
//    ui->rCountMinSpinBox->setValue(1);
//    int maxc = ui->profileTreeWidget->topLevelItemCount() - 1;
//    ui->rCountMaxSpinBox->setValue(maxc);

//    ui->rCountMinSpinBox->setMaximum(maxc);
//    ui->rCountMaxSpinBox->setMaximum(maxc);
}
