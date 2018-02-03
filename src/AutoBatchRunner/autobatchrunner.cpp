#include "autobatchrunner.h"
#include "ui_autobatchrunner.h"


AutoBatchRunner::AutoBatchRunner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //set dockwidget occupied area
//    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

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
    ui->consoleDock->setAutohide(ui->actionAutohide->isChecked());

    //set menu show
    connect(ui->actionAutohide, &QAction::triggered, ui->consoleDock, &BaseDockWidget::setAutohide);
    connect(ui->consoleDock, &BaseDockWidget::visibilityChanged, ui->actionAutohide, &QAction::setChecked);

    //init optiondialog object
    opdialog = new OptionDialog();

    //init ui add delete edit button
    connect(ui->addButton, &QToolButton::clicked, ui->comboBox, &ProfileComboBox::addItemAction);
    connect(ui->deleteButton, &QToolButton::clicked, ui->comboBox, &ProfileComboBox::deleteItemAction);

    //init profilelist
    ui->comboBox->reloadComboBoxItem();

    initStatusBar();
}

AutoBatchRunner::~AutoBatchRunner()
{
    //save window state
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.setValue( "main/geometry", saveGeometry() );
    settings.setValue( "main/windowState", saveState() );

    //delete object
    delete opdialog;
//    delete sysTray;
    delete ui;
}

void AutoBatchRunner::on_actionOpen_triggered()
{
    ui->fileTable->addAction();
}

void AutoBatchRunner::on_actionSettings_triggered()
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

void AutoBatchRunner::initStatusBar()
{
    QLabel *label = new QLabel();
    label->setAlignment(Qt::AlignLeft);
    ui->statusBar->addPermanentWidget(label, 3);
//    connect(this, SIGNAL(rowPosChanged(QString)), label, SLOT(setText(QString)));

    QProgressBar *progressbar = new QProgressBar();
    progressbar->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(progressbar, 1);
//    connect(rbinder, &RunTaskSignalBinder::processInitCount, progressbar, &QProgressBar::setRange);
//    connect(rbinder, &RunTaskSignalBinder::processCurrent, progressbar, &QProgressBar::setValue);
}
