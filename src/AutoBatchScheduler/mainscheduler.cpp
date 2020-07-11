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
    setWindowTitle("ProfileScheduler");

    //set window icon
    setWindowIcon(QIcon(":/app_icons/app_abs_24x24.ico"));

    //set toolbar buttons
    connect(ui->actionExit, &QAction::triggered, this, &MainScheduler::close);
    connect(ui->actionSchedule, &QAction::triggered, this, &MainScheduler::onSchedulerButtonClicked);
    connect(ui->actionOption, &QAction::triggered, this, &MainScheduler::onOptionButtonClicked);

    //Help
    connect(ui->actionAbout, &QAction::triggered, this, &MainScheduler::about);

    //restore settings
    restoreSettings();

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
    saveSettings();

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

void MainScheduler::saveSettings()
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
    settings.setValue("WINDOWFONT", ui->windowFontComboBox->currentText());
    settings.setValue("WINDOWFONTSIZE", ui->windowFontSizeSpinBox->value());
    settings.endGroup();
}

void MainScheduler::restoreSettings()
{
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
    ui->windowFontComboBox->setCurrentFont(QFont(settings.value("WINDOWFONT", QApplication::font().toString()).toString()));
    ui->windowFontSizeSpinBox->setValue(settings.value("WINDOWFONTSIZE", QApplication::font().pointSize()).toInt());
    settings.endGroup();
}

//QSS_THEME
void MainScheduler::themeChangeAction()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("scheduler_startup");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
    QFont settingfont = QFont(settings.value("WINDOWFONT", QApplication::font().toString()).toString());
    settingfont.setPointSize(settings.value("WINDOWFONTSIZE", QApplication::font().pointSize()).toInt());
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
            ui->startupTableWidget->setStyleSheet(data);
        }
    }

    QApplication::setFont(settingfont);
}

void MainScheduler::about()
{
    AboutPS *ab = new AboutPS;
    ab->setStyleSheet(this->styleSheet());
    ab->move(this->geometry().center() - ab->rect().center());
    ab->show();
}

