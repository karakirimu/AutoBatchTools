#include "autobatchrunner.h"
#include "ui_autobatchrunner.h"

///
/// \brief AutoBatchRunner::AutoBatchRunner This is a main constructor of AutoBatchRunner.exe
/// \param parent default QMainWindow
///
AutoBatchRunner::AutoBatchRunner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoBatchRunner)
{
    //set dockwidget occupied area
//    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //setup ui
    ui->setupUi(this);

    //init optiondialog object
    opdialog = new OptionDialog();

    //init task management
    mlTask = new MultiTask();

    //set window icon
    setWindowIcon(QIcon(":/app_icons/app_abr_24x24.ico"));

    //init profilelist
    ui->comboBox->reloadComboBoxItem();

    //Window init
    setWindowTitle(tr("AutoBatchRunner - BatchRunner"));

    QSettings settings( "./settings.ini", QSettings::IniFormat );
    QVariant v = settings.value( "batchrunner/geometry" );
    if (v.type() != QVariant::Invalid){
        // load window settings on MainWindow
        restoreGeometry( settings.value( "batchrunner/geometry" ).toByteArray() );
        restoreState( settings.value( "batchrunner/windowState" ).toByteArray() );

        // load combobox previous position
        int comboindex = settings.value("batchrunner/profilecombo").toInt();
        if(comboindex < ui->comboBox->count()){
            ui->comboBox->setCurrentIndex(comboindex);
        }
    }

    //init ui theme
    themeChangeAction();

    //set menu action
    connect(ui->actionExit, &QAction::triggered, qApp, &QCoreApplication::quit);

    //set dock autohide TODO: not saved menu
    ui->consoleDock->setAutohide(ui->actionAutohide->isChecked());

    //set menu show
    connect(ui->actionAutohide, &QAction::triggered, ui->consoleDock, &BaseDockWidget::setAutohide);
    connect(ui->consoleDock, &BaseDockWidget::visibilityChanged, ui->actionAutohide, &QAction::setChecked);

    //init ui add delete edit button
    connect(ui->profileAddToolButton, &QToolButton::clicked, ui->comboBox, &ProfileComboBox::addItemAction);
    connect(ui->profileDeleteToolButton, &QToolButton::clicked, ui->comboBox, &ProfileComboBox::deleteItemAction);

    initStatusBar();

    //to send to console
    ui->console->setMultiTask(mlTask);
    ui->commandSender->setMultiTask(mlTask);

    connect(mlTask, &MultiTask::processStarted, this, &AutoBatchRunner::taskStarted);
    connect(mlTask, &MultiTask::processPaused, this, &AutoBatchRunner::taskPaused);
    connect(mlTask, &MultiTask::processStopped, this, &AutoBatchRunner::taskStopped);
    connect(mlTask, &MultiTask::processEnd, this, &AutoBatchRunner::taskEnd);
}

AutoBatchRunner::~AutoBatchRunner()
{
    //save window state
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.setValue( "batchrunner/geometry", saveGeometry() );
    settings.setValue( "batchrunner/windowState", saveState() );
    settings.setValue( "batchrunner/profilecombo", ui->comboBox->currentIndex());

    delete opdialog;
    delete mlTask;
    delete ui;
}

void AutoBatchRunner::taskStarted(QString objectname, int runfrom)
{
    Q_UNUSED(objectname)
    Q_UNUSED(runfrom)
    setRunButtonState(false, true, true);
}

void AutoBatchRunner::taskPaused(QString objectname)
{
    Q_UNUSED(objectname)
    setRunButtonState(true, false, true);
}

void AutoBatchRunner::taskStopped(QString objectname)
{
    Q_UNUSED(objectname)
    setRunButtonState(true, false, false);
}

void AutoBatchRunner::taskEnd(QString objectname, int runfrom)
{
    Q_UNUSED(objectname)
    Q_UNUSED(runfrom)
    setRunButtonState(true, false, false);

    mlTask->removeTask(key);
    key = "";
    ui->console->setReadObjectName(key);
    ui->commandSender->setObjectName(key);
}

void AutoBatchRunner::on_actionOpen_triggered()
{
    ui->fileTable->addFilesAction();
}

void AutoBatchRunner::on_actionRun_triggered()
{
    setRunButtonState(false, false, false);

    if(key == ""){
        key = mlTask->generateRandom(32);
        mlTask->addTask(key, ui->comboBox->getCurrentFileName());
        ui->console->setReadObjectName(key);
        ui->commandSender->setObjectName(key);

        //get filelist
        QStringList flist;
        int filecount = ui->fileTable->rowCount();
        for(int i = 0; i < filecount; i++){
            flist.append(ui->fileTable->item(i,0)->text());
        }

        mlTask->setInputFileList(key, &flist);

    }else{
        mlTask->processPause(key);
    }
}

void AutoBatchRunner::on_actionPause_triggered()
{
    setRunButtonState(false, false, false);

    mlTask->processPause(key);
}

void AutoBatchRunner::on_actionStop_triggered()
{
    setRunButtonState(true, false, false);

    mlTask->removeTask(key);
    key = "";
    ui->console->setReadObjectName(key);
    ui->commandSender->setObjectName(key);
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
    connect(mlTask, &MultiTask::processInitCount, progressbar, &QProgressBar::setRange);
    connect(mlTask, &MultiTask::processCurrent, progressbar, &QProgressBar::setValue);
}

void AutoBatchRunner::on_profileEditToolButton_clicked()
{
    //run ProfileEditor.exe
    QProcess process;
#ifdef QT_DEBUG
    bool result = process.startDetached("./ProfileEditor.exe", \
                    (QStringList() << ui->comboBox->getCurrentFileName()));
    if(!result) qDebug() << tr("ProfileEditor launch failed.");
#else
#ifdef Q_OS_WIN
    process.startDetached("./ProfileEditor.exe", QStringList() << ui->comboBox->getCurrentFileName());
#else
    process.startDetached("./ProfileEditor", QStringList() << ui->comboBox->getCurrentFileName());
#endif
#endif
}

void AutoBatchRunner::setRunButtonState(bool run, bool pause, bool stop)
{
    ui->actionRun->setEnabled(run);
    ui->actionPause->setEnabled(pause);
    ui->actionStop->setEnabled(stop);
}

//QSS_THEME
void AutoBatchRunner::themeChangeAction()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("abr_settings");
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
            opdialog->setStyleSheet(data);
            ui->fileTable->setStyleSheet(data);
        }
    }

    QApplication::setFont(settingfont);
}
