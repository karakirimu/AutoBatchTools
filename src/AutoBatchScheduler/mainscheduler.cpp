/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
    setWindowTitle("AutoBatchScheduler");

    //set window icon
    setWindowIcon(QIcon(":/app_icons/app_abs.ico"));

    //update language combobox
    initLanguageSelection();

    //set toolbar buttons
    connect(ui->actionExit, &QAction::triggered, this, &MainScheduler::close);
    connect(ui->actionSchedule, &QAction::triggered, this, &MainScheduler::onSchedulerButtonClicked);
    connect(ui->actionOption, &QAction::triggered, this, &MainScheduler::onOptionButtonClicked);

    //Help
    connect(ui->actionAbout, &QAction::triggered, this, &MainScheduler::about);

    //restore settings
    restoreSettings();

    //make taskscheduler
    taskscheduler = new TaskSchedulerConnector(this);

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

    //load xml
    taskscheduler->loadFile();

    sysTray->show();
}

MainScheduler::~MainScheduler()
{   
    sysTray->hide();
    delete sysTray;
    delete ui;
    delete taskscheduler;
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
//    ui->startupTableWidget->reloadAction();

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
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );

    settings.beginGroup(sc.GROUP_ABS);
    //message tab
    settings.setValue(sc.ABS_MINIMIZE, ui->minimizeShowCheckBox->isChecked());
    settings.setValue(sc.ABS_MINIMIZE_MS, ui->minimizeSpinBox->value());

    settings.setValue(sc.ABS_TIMERSTART, ui->timerStartShowCheckBox->isChecked());
    settings.setValue(sc.ABS_TIMERSTART_MS, ui->timerStartSpinBox->value());
    settings.setValue(sc.ABS_TIMERSTOP, ui->timerStopShowCheckBox->isChecked());
    settings.setValue(sc.ABS_TIMERSTOP_MS, ui->timerStopSpinBox->value());

    settings.setValue(sc.ABS_TASKSTART, ui->taskStartShowCheckBox->isChecked());
    settings.setValue(sc.ABS_TASKSTART_MS, ui->taskStartSpinBox->value());
    settings.setValue(sc.ABS_TASKFINISHED, ui->taskEndShowCheckBox->isChecked());
    settings.setValue(sc.ABS_TASKFINISHED_MS, ui->taskEndSpinBox->value());

    settings.setValue(sc.ABS_TASKUNSELECTED, ui->taskUnselectShowCheckBox->isChecked());
    settings.setValue(sc.ABS_TASKUNSELECTED_MS, ui->taskUnselectSpinBox->value());

    settings.setValue(sc.ABS_MINIMIZE_WINDOW, ui->minimizeLaunchCheckBox->isChecked());
    settings.setValue(sc.ABS_THEME, ui->themeComboBox->currentText());
    settings.setValue(sc.ABS_FONT, ui->windowFontComboBox->currentText());
    settings.setValue(sc.ABS_FONTSIZE, ui->windowFontSizeSpinBox->value());

    storeLanguageSelection(&settings);

    settings.endGroup();
}

void MainScheduler::restoreSettings()
{
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
    settings.beginGroup(sc.GROUP_ABS);
    //restore settings
    ui->minimizeShowCheckBox->setChecked(settings.value(sc.ABS_MINIMIZE, true).toBool());
    ui->minimizeSpinBox->setValue(settings.value(sc.ABS_MINIMIZE_MS, 2500).toInt());

    ui->timerStartShowCheckBox->setChecked(settings.value(sc.ABS_TIMERSTART, true).toBool());
    ui->timerStartSpinBox->setValue(settings.value(sc.ABS_TIMERSTART_MS, 2500).toInt());
    ui->timerStopShowCheckBox->setChecked(settings.value(sc.ABS_TIMERSTOP, true).toBool());
    ui->timerStopSpinBox->setValue(settings.value(sc.ABS_TIMERSTOP_MS, 2500).toInt());

    ui->taskStartShowCheckBox->setChecked(settings.value(sc.ABS_TASKSTART, true).toBool());
    ui->taskStartSpinBox->setValue(settings.value(sc.ABS_TASKSTART_MS, 2500).toInt());
    ui->taskEndShowCheckBox->setChecked(settings.value(sc.ABS_TASKFINISHED, true).toBool());
    ui->taskEndSpinBox->setValue(settings.value(sc.ABS_TASKFINISHED_MS, 2500).toInt());

    ui->taskUnselectShowCheckBox->setChecked(settings.value(sc.ABS_TASKUNSELECTED, true).toBool());
    ui->taskUnselectSpinBox->setValue(settings.value(sc.ABS_TASKUNSELECTED_MS, 2500).toInt());

    ui->minimizeLaunchCheckBox->setChecked(settings.value(sc.ABS_MINIMIZE_WINDOW, false).toBool());
    ui->themeComboBox->setCurrentText(settings.value(sc.ABS_THEME, "Default").toString());
    ui->windowFontComboBox->setCurrentFont(QFont(settings.value(sc.ABS_FONT, QApplication::font().toString()).toString()));
    ui->windowFontSizeSpinBox->setValue(settings.value(sc.ABS_FONTSIZE, QApplication::font().pointSize()).toInt());

    loadLanguageSelection(&settings);

    settings.endGroup();
}

//QSS_THEME
void MainScheduler::themeChangeAction()
{
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );

    //theme settings
    settings.beginGroup(sc.GROUP_ABS);
    QString stylecolor = settings.value(sc.ABS_THEME, "Default").toString();
    QFont settingfont = QFont(settings.value(sc.ABS_FONT, QApplication::font().toString()).toString());
    settingfont.setPointSize(settings.value(sc.ABS_FONTSIZE, QApplication::font().pointSize()).toInt());
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
    AboutABS *ab = new AboutABS;
    ab->setStyleSheet(this->styleSheet());
    ab->move(this->geometry().center() - ab->rect().center());
    ab->show();
}

void MainScheduler::initLanguageSelection()
{
#ifdef QT_DEBUG
    QDirIterator dit("../src/translation", QStringList() << "*.qm", QDir::Files);
#else
    QDirIterator dit("translation", QStringList() << "*.qm", QDir::Files);
#endif

    while (dit.hasNext()){
        QFileInfo file(dit.next());
        QString strloc = file.baseName().split("_").last();
        QLocale locale(strloc);
        ui->languageComboBox->addItem(QLocale::languageToString(locale.language()));
    }
}

void MainScheduler::storeLanguageSelection(QSettings *setting)
{
    QString selected = ui->languageComboBox->currentText();

    QList<QLocale> locales = QLocale::matchingLocales(QLocale::AnyLanguage,
                                                      QLocale::AnyScript,
                                                      QLocale::AnyCountry);

    for(const QLocale& loc : locales){
        if(QLocale::languageToString(loc.language()) == selected){
            setting->setValue(sc.ABS_LANGUAGE, loc.bcp47Name());
            break;
        }
    }
}

void MainScheduler::loadLanguageSelection(QSettings *setting)
{
    QLocale defloc;
    QLocale locale(setting->value(sc.ABS_LANGUAGE, defloc.bcp47Name()).toString());
    ui->languageComboBox->setCurrentText(QLocale::languageToString(locale.language()));
}

