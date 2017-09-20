#include "mainscheduler.h"
#include "ui_mainscheduler.h"

MainScheduler::MainScheduler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScheduler)
{
    ui->setupUi(this);

    //initial selected position
    ui->optionStackedWidget->setCurrentIndex(0);
    ui->actionSchedule->setChecked(true);

    //set toolbar buttons
    connect(ui->actionExit, &QAction::triggered, this, &MainScheduler::hide);
    connect(ui->actionSchedule, &QAction::triggered, this, &MainScheduler::onSchedulerButtonClicked);
    connect(ui->actionOption, &QAction::triggered, this, &MainScheduler::onOptionButtonClicked);

    //restore settings
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("STARTUP");
    //restore settings
    ui->timerStartShowCheckBox->setChecked(settings.value("HIDETIMERSTART", false).toBool());
    ui->timerStartSpinBox->setValue(settings.value("TIMERSTARTMS", 2500).toInt());
    ui->taskStartShowCheckBox->setChecked(settings.value("HIDETASKSTART", false).toBool());
    ui->taskStartSpinBox->setValue(settings.value("TASKSTARTMS", 2500).toInt());
    ui->taskEndShowCheckBox->setChecked(settings.value("HIDETASKEND", false).toBool());
    ui->minimizeLaunchCheckBox->setChecked(settings.value("HIDEWINDOW", false).toBool());
    settings.endGroup();

    //make taskscheduler
    taskscheduler = new TaskSchedulerConnector();

    //launch systemtray
    sysTray = new SystemTray();
    sysTray->setTaskSchedulerConnector(taskscheduler);

    //set to startuptable
    ui->startupTableWidget->setTaskSchedulerConnector(taskscheduler);

    //connect systray actions
    connect(sysTray, &SystemTray::launchSetting, this, &MainScheduler::showWindow);
    connect(sysTray, &SystemTray::launchclose, qApp, &QApplication::quit);

    sysTray->show();
}

MainScheduler::~MainScheduler()
{   
    sysTray->hide();
    delete sysTray;
    delete taskscheduler;
    delete ui;
}

void MainScheduler::onSchedulerButtonClicked()
{
    ui->optionStackedWidget->setCurrentIndex(0);
    ui->actionSchedule->setChecked(true);
    ui->actionOption->setChecked(false);
}

void MainScheduler::onOptionButtonClicked()
{
    ui->optionStackedWidget->setCurrentIndex(1);
    ui->actionSchedule->setChecked(false);
    ui->actionOption->setChecked(true);
}

void MainScheduler::showWindow()
{
    this->isMinimized() ? this->showNormal() : this->show();
}

void MainScheduler::closeEvent(QCloseEvent *event)
{
    //save settings when close window
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("STARTUP");
    //message tab
    settings.setValue("HIDETIMERSTART", ui->timerStartShowCheckBox->isChecked());
    settings.setValue("TIMERSTARTMS", ui->timerStartSpinBox->value());
    settings.setValue("HIDETASKSTART", ui->taskStartShowCheckBox->isChecked());
    settings.setValue("TASKSTARTMS", ui->taskStartSpinBox->value());
    settings.setValue("HIDETASKEND", ui->taskEndShowCheckBox->isChecked());
    settings.setValue("HIDEWINDOW", ui->minimizeLaunchCheckBox->isChecked());
    settings.endGroup();

#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif

    //hide and left tasktray.
    //show message
    sysTray->showNotCloseMessage();
    this->hide();
    event->ignore();
}
