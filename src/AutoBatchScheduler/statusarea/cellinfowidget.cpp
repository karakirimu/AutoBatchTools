#include "cellinfowidget.h"
#include "ui_cellinfowidget.h"

cellInfoWidget::cellInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cellInfoWidget)
{
    ui->setupUi(this);

    //init
    ui->progressBar->setValue(0);
    connect(ui->consoleToolButton, &QToolButton::clicked, this, &cellInfoWidget::onConsoleButtonClicked);
    connect(ui->pauseToolButton, &QToolButton::clicked, this, &cellInfoWidget::onPauseButtonClicked);
    connect(ui->stopToolButton, &QToolButton::clicked, this, &cellInfoWidget::onStopButtonClicked);
}

cellInfoWidget::~cellInfoWidget()
{
    delete ui;
}

void cellInfoWidget::setProfileName(QString name)
{
    ui->profileNameLabel->setText(name);
}

void cellInfoWidget::started()
{
    ui->progressBar->setEnabled(true);
    ui->consoleToolButton->setEnabled(true);
    ui->pauseToolButton->setEnabled(true);
    ui->stopToolButton->setEnabled(true);
}

void cellInfoWidget::scheduled()
{
    ui->progressBar->setEnabled(false);
    ui->consoleToolButton->setEnabled(false);
    ui->pauseToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
}

void cellInfoWidget::updateProcess(QString data, int type)
{
    Q_UNUSED(type);
    ui->processLabel->setText(data);
}

void cellInfoWidget::updateProgress(int num)
{
    ui->progressBar->setValue(num);
}

void cellInfoWidget::setProgressminmax(int start, int end)
{
    ui->progressBar->setRange(start, end);
}

void cellInfoWidget::onConsoleButtonClicked()
{
    consoleButtonClicked(this->objectName());
}

void cellInfoWidget::onPauseButtonClicked()
{
    pauseButtonClicked(this->objectName());
}

void cellInfoWidget::onStopButtonClicked()
{
    stopButtonClicked(this->objectName());
}
