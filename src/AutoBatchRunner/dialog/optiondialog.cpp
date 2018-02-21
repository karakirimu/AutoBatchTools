#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    setWindowFlags(Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    setupItem();
    loadSettings();

    //connect action
    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
//    connect( ui->buttonBox, SIGNAL(rejected()), this, SLOT(onReject()));
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setupItem(){
    ui->listWidget->addItem(tr("General"));
    ui->listWidget->addItem(tr("Profile"));
//    ui->listWidget->addItem(tr("Resident"));
    ui->listWidget->addItem(tr("Extra Functions"));
//    ui->listWidget->addItem(tr("Theme"));

    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget->setIconSize(QSize(32,32));

    ui->listWidget->item(0)->setIcon(QIcon(":/default_icons/settings.png"));
    ui->listWidget->item(1)->setIcon(QIcon(":/default_icons/others.png"));
//    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/app_abs.png"));
    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/extras.png"));
//    ui->listWidget->item(3)->setIcon(QIcon(":/icons/Colors.png"));
}

void OptionDialog::setSettings()
{
//    QSettings settings( "./settings.ini", QSettings::IniFormat );

//    settings.beginGroup("STARTUP");
//    settings.setValue("ENABLED", ui->sysTrayCheckBox->isChecked());
//    settings.setValue("STARTUPM", ui->startupCheckBox->isChecked());
//    settings.setValue("ALLCLOSE", ui->allcloseCheckBox->isChecked());
//    settings.setValue("HIDETIMERSTART", ui->timerStartCheckBox->isChecked());
//    settings.setValue("TIMERSTARTMS", ui->timerStartSpinBox->value());
//    settings.setValue("HIDETASKSTART", ui->taskStartCheckBox->isChecked());
//    settings.setValue("TASKSTARTMS", ui->taskStartSpinBox->value());
//    settings.endGroup();
}

void OptionDialog::loadSettings()
{
//    QSettings settings( "./settings.ini", QSettings::IniFormat );

//    settings.beginGroup("STARTUP");
//    ui->sysTrayCheckBox->setChecked(settings.value("ENABLED", false).toBool());
//    ui->startupCheckBox->setChecked(settings.value("STARTUPM", false).toBool());
//    ui->allcloseCheckBox->setChecked(settings.value("ALLCLOSE", true).toBool());
//    ui->timerStartCheckBox->setChecked(settings.value("HIDETIMERSTART", false).toBool());
//    ui->timerStartSpinBox->setValue(settings.value("TIMERSTARTMS", 2500).toInt());
//    ui->taskStartCheckBox->setChecked(settings.value("HIDETASKSTART", false).toBool());
//    ui->taskStartSpinBox->setValue(settings.value("TASKSTARTMS", 2500).toInt());
//    settings.endGroup();
}

//change list to stackedwidget
void OptionDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void OptionDialog::onAccept()
{
    setSettings();
//    systemTraySelect();
}

//void OptionDialog::onReject()
//{
//    systemTraySelect();
//}

//void OptionDialog::closeEvent(QCloseEvent *event)
//{
//    systemTraySelect() ? event->ignore() : event->accept();
//}

//bool OptionDialog::systemTraySelect()
//{
//#ifdef Q_OS_OSX
//    if (!event->spontaneous() || !isVisible()) {
//        return;
//    }
//#endif
//    //when startup action selected, hide window.
//    QSettings settings( "./settings.ini", QSettings::IniFormat );
//    settings.beginGroup("STARTUP");
//    bool isstartup = settings.value("ENABLED", false).toBool();
//    settings.endGroup();

//    if(isstartup) this->hide();

//    return isstartup;
//}
