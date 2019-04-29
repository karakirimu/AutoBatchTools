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

    //project
    connect(ui->pAddButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::addAction);
    connect(ui->pDeleteButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::deleteAction);
    connect(ui->pUpButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::upAction);
    connect(ui->pDownButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::downAction);
    connect(ui->pEditButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::editAction);

    //application variant
    connect(ui->vAddButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::addAction);
    connect(ui->vCopyButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::copyAction);
    connect(ui->vDeleteButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::deleteAction);
    connect(ui->vUpButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::upAction);
    connect(ui->vDownButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::downAction);

    //search variant
    connect(ui->sAddButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::addAction);
    connect(ui->sCopyButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::copyAction);
    connect(ui->sDeleteButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::deleteAction);
    connect(ui->sDownButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::upAction);
    connect(ui->sUpButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::downAction);

    //extend variant
    connect(ui->eAddButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::addAction);
    connect(ui->eDeleteButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::deleteAction);
    connect(ui->eUpButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::upAction);
    connect(ui->eDownButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::downAction);

}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setupItem(){
    ui->listWidget->addItem(tr("General"));
    ui->listWidget->addItem(tr("Project"));
//    ui->listWidget->addItem(tr("Resident"));
    ui->listWidget->addItem(tr("Global Variant"));
    ui->listWidget->addItem(tr("Search"));
    ui->listWidget->addItem(tr("Plugins"));
//    ui->listWidget->addItem(tr("Theme"));

    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget->setIconSize(QSize(32,32));

    ui->listWidget->item(0)->setIcon(QIcon(":/default_icons/settings.png"));
    ui->listWidget->item(1)->setIcon(QIcon(":/default_icons/others.png"));
//    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/app_abs.png"));
    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/string.png"));
    ui->listWidget->item(3)->setIcon(QIcon(":/default_icons/search.png"));
    ui->listWidget->item(4)->setIcon(QIcon(":/default_icons/extras.png"));
//    ui->listWidget->item(3)->setIcon(QIcon(":/icons/Colors.png"));
}

void OptionDialog::setSettings()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("abr_settings");
    settings.setValue("THEMECOLOR", ui->themeComboBox->currentText());
//    settings.setValue("ENABLED", ui->sysTrayCheckBox->isChecked());
//    settings.setValue("STARTUPM", ui->startupCheckBox->isChecked());
//    settings.setValue("ALLCLOSE", ui->allcloseCheckBox->isChecked());
//    settings.setValue("HIDETIMERSTART", ui->timerStartCheckBox->isChecked());
//    settings.setValue("TIMERSTARTMS", ui->timerStartSpinBox->value());
//    settings.setValue("HIDETASKSTART", ui->taskStartCheckBox->isChecked());
//    settings.setValue("TASKSTARTMS", ui->taskStartSpinBox->value());
    settings.endGroup();
}

void OptionDialog::loadSettings()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("abr_settings");
    ui->themeComboBox->setCurrentText(settings.value("THEMECOLOR", "Default").toString());
//    ui->sysTrayCheckBox->setChecked(settings.value("ENABLED", false).toBool());
//    ui->startupCheckBox->setChecked(settings.value("STARTUPM", false).toBool());
//    ui->allcloseCheckBox->setChecked(settings.value("ALLCLOSE", true).toBool());
//    ui->timerStartCheckBox->setChecked(settings.value("HIDETIMERSTART", false).toBool());
//    ui->timerStartSpinBox->setValue(settings.value("TIMERSTARTMS", 2500).toInt());
//    ui->taskStartCheckBox->setChecked(settings.value("HIDETASKSTART", false).toBool());
//    ui->taskStartSpinBox->setValue(settings.value("TASKSTARTMS", 2500).toInt());
    settings.endGroup();
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
//    settings.beginGroup("scheduler_startup");
//    bool isstartup = settings.value("ENABLED", false).toBool();
//    settings.endGroup();

//    if(isstartup) this->hide();

//    return isstartup;
//}
