#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
//    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowCloseButtonHint);

    ui->setupUi(this);
    setupItem();
    loadSettings();

    //connect action
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
//    connect( ui->buttonBox, SIGNAL(rejected()), this, SLOT(onReject()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonBoxClicked(QAbstractButton*)));
    connect(ui->openButton, &QToolButton::clicked, this, &SettingDialog::openDir);

    //global variant
    connect(ui->gAddButton, &QPushButton::clicked, ui->stringTableWidget, &StringTable::addAction);
    connect(ui->gCopyButton, &QPushButton::clicked, ui->stringTableWidget, &StringTable::copyAction);
    connect(ui->gDeleteButton, &QPushButton::clicked, ui->stringTableWidget, &StringTable::deleteAction);
    connect(ui->gUpButton, &QPushButton::clicked, ui->stringTableWidget, &StringTable::upAction);
    connect(ui->gDownButton, &QPushButton::clicked, ui->stringTableWidget, &StringTable::downAction);

    //search variant
    connect(ui->seAddButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::addAction);
    connect(ui->seCopyButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::copyAction);
    connect(ui->seDeleteButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::deleteAction);
    connect(ui->seDownButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::upAction);
    connect(ui->seUpButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::downAction);

    //extrafunc variant
    connect(ui->exAddButton, &QPushButton::clicked, ui->extrasTableWidget, &ExtraFunctionsTable::addAction);
    connect(ui->exDeleteButton, &QPushButton::clicked, ui->extrasTableWidget, &ExtraFunctionsTable::deleteAction);
    connect(ui->exCopyButton, &QPushButton::clicked, ui->extrasTableWidget, &ExtraFunctionsTable::copyAction);
    connect(ui->exOpenButton, &QPushButton::clicked, ui->extrasTableWidget, &ExtraFunctionsTable::openFileAction);

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::setupItem(){
    ui->listWidget->addItem(tr("General"));
    ui->listWidget->addItem(tr("Variant"));
    ui->listWidget->addItem(tr("Search"));
    ui->listWidget->addItem(tr("Extra Function"));
//    ui->listWidget->addItem(tr("Theme"));

    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget->setIconSize(QSize(32,32));

    ui->listWidget->item(0)->setIcon(QIcon(":/default_icons/settings.png"));
    ui->listWidget->item(1)->setIcon(QIcon(":/default_icons/string.png"));
    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/search.png"));
    ui->listWidget->item(3)->setIcon(QIcon(":/default_icons/extras.png"));
//    ui->listWidget->item(4)->setIcon(QIcon(":/icons/Colors.png"));
}

void SettingDialog::setSettings()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("pe_general");
    settings.setValue("TEMPDIR", ui->tempEdit->text());
    settings.setValue("AUTOSAVEPERIOD", ui->autosaveSpinBox->value());
    settings.setValue("THEMECOLOR", ui->themeComboBox->currentText());
    settings.endGroup();

    settings.beginGroup("pe_testexec");
    settings.setValue("DETACH", ui->detachCheckBox->isChecked());
    settings.setValue("FAKERES", ui->fakeresidentCheckBox->isChecked());
    settings.setValue("FSUPDATE", ui->fsupdateCheckBox->isChecked());
//    settings.setValue("TIMEOUT", ui->timeoutSpinBox->value());
//    settings.setValue("FILELOADMAX", ui->fileloadSpinBox->value());
    settings.endGroup();

//    settings.beginGroup("scheduler_startup");
//    settings.setValue("ENABLED", ui->sysTrayCheckBox->isChecked());
//    settings.setValue("STARTUPM", ui->startupCheckBox->isChecked());
//    settings.setValue("ALLCLOSE", ui->allcloseCheckBox->isChecked());
//    settings.setValue("HIDETIMERSTART", ui->timerStartCheckBox->isChecked());
//    settings.setValue("TIMERSTARTMS", ui->timerStartSpinBox->value());
//    settings.setValue("HIDETASKSTART", ui->taskStartCheckBox->isChecked());
//    settings.setValue("TASKSTARTMS", ui->taskStartSpinBox->value());
//    settings.endGroup();
}

void SettingDialog::loadSettings()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("pe_general");
    ui->tempEdit->setText(settings.value("TEMPDIR", "./").toString());
    ui->autosaveSpinBox->setValue(settings.value("AUTOSAVEPERIOD", 5).toInt());
    ui->themeComboBox->setCurrentText(settings.value("THEMECOLOR", "Default").toString());
    settings.endGroup();

    settings.beginGroup("pe_testexec");
    ui->detachCheckBox->setChecked(settings.value("DETACH", false).toBool());
    ui->fakeresidentCheckBox->setChecked(settings.value("FAKERES", false).toBool());
    ui->fsupdateCheckBox->setChecked(settings.value("FSUPDATE", true).toBool());
//    ui->timeoutSpinBox->setValue(settings.value("TIMEOUT", 30000).toInt());
//    ui->fileloadSpinBox->setValue(settings.value("FILELOADMAX", 10).toInt());
    settings.endGroup();

//    settings.beginGroup("scheduler_startup");
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
void SettingDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
    if(currentRow == 2) ui->searchTableWidget->setStyleSheet(this->styleSheet());
}

void SettingDialog::onAccept()
{
    setSettings();
}

void SettingDialog::onButtonBoxClicked(QAbstractButton *button)
{
    if(button->text() == tr("Apply")){
        setSettings();
    }
}

void SettingDialog::openDir()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString res = dialog.getExistingDirectory(this, tr("Open Folder"), ui->tempEdit->text());
    ui->tempEdit->setText(res);
}

//void SettingDialog::onReject()
//{
////    systemTraySelect();
//}

void SettingDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    this->hide();
//    systemTraySelect() ? event->ignore() : event->accept();
}

//bool SettingDialog::systemTraySelect()
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
