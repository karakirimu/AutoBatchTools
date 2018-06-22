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
    setWindowTitle(tr("AutoBatchRunner - Scheduler"));

    //set toolbar buttons
    connect(ui->actionExit, &QAction::triggered, this, &MainScheduler::close);
    connect(ui->actionSchedule, &QAction::triggered, this, &MainScheduler::onSchedulerButtonClicked);
    connect(ui->actionOption, &QAction::triggered, this, &MainScheduler::onOptionButtonClicked);

    //restore settings
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("scheduler_startup");
    //restore settings
    ui->minimizeShowCheckBox->setChecked(settings.value("MINIMIZESHOW", true).toBool());
    ui->minimizeSpinBox->setValue(settings.value("MINIMIZESHOWMS", 2500).toInt());

    ui->timerStartShowCheckBox->setChecked(settings.value("TIMERSTART", true).toBool());
    ui->timerStartSpinBox->setValue(settings.value("TIMERSTARTMS", 2500).toInt());
    ui->timerStopShowCheckBox->setChecked(settings.value("TIMERSTOP", true).toBool());
    ui->timerStopSpinBox->setValue(settings.value("TIMERSTOPMS", 2500).toInt());

    ui->taskStartShowCheckBox->setChecked(settings.value("TASKSTART", true).toBool());
    ui->taskStartSpinBox->setValue(settings.value("TASKSTARTMS", 2500).toInt());
    ui->taskEndShowCheckBox->setChecked(settings.value("TASKEND", true).toBool());
    ui->taskEndSpinBox->setValue(settings.value("TASKENDMS", 2500).toInt());

    ui->taskUnselectShowCheckBox->setChecked(settings.value("TASKUNSELECT", true).toBool());
    ui->taskUnselectSpinBox->setValue(settings.value("TASKUNSELECTMS", 2500).toInt());

    ui->minimizeLaunchCheckBox->setChecked(settings.value("HIDEWINDOW", false).toBool());
    ui->themeComboBox->setCurrentText(settings.value("THEMECOLOR", "Default").toString());
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

    //init ui theme
    themeChangeAction();

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
    //reflection startuptable
    ui->startupTableWidget->reloadAction();

    this->isMinimized() ? this->showNormal() : this->show();
}

void MainScheduler::closeEvent(QCloseEvent *event)
{
    //save settings when close window
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("scheduler_startup");
    //message tab
    settings.setValue("MINIMIZESHOW", ui->minimizeShowCheckBox->isChecked());
    settings.setValue("MINIMIZESHOWMS", ui->minimizeSpinBox->value());

    settings.setValue("TIMERSTART", ui->timerStartShowCheckBox->isChecked());
    settings.setValue("TIMERSTARTMS", ui->timerStartSpinBox->value());
    settings.setValue("TIMERSTOP", ui->timerStopShowCheckBox->isChecked());
    settings.setValue("TIMERSTOPMS", ui->timerStopSpinBox->value());

    settings.setValue("TASKSTART", ui->taskStartShowCheckBox->isChecked());
    settings.setValue("TASKSTARTMS", ui->taskStartSpinBox->value());
    settings.setValue("TASKEND", ui->taskEndShowCheckBox->isChecked());
    settings.setValue("TASKENDMS", ui->taskEndSpinBox->value());

    settings.setValue("TASKUNSELECT", ui->taskUnselectShowCheckBox->isChecked());
    settings.setValue("TASKUNSELECTMS", ui->taskUnselectSpinBox->value());

    settings.setValue("HIDEWINDOW", ui->minimizeLaunchCheckBox->isChecked());
    settings.setValue("THEMECOLOR", ui->themeComboBox->currentText());
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

//QSS_THEME
void MainScheduler::themeChangeAction()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("scheduler_startup");
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

