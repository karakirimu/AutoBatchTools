#include "cellinfowidget.h"
#include "ui_cellinfowidget.h"

CellInfoWidget::CellInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cellInfoWidget)
{
    ui->setupUi(this);

    //init
    ui->progressBar->setValue(0);
    connect(ui->consoleToolButton, &QToolButton::clicked, this, &CellInfoWidget::onConsoleButtonClicked);
    connect(ui->pauseToolButton, &QToolButton::clicked, this, &CellInfoWidget::onPauseButtonClicked);
    connect(ui->stopToolButton, &QToolButton::clicked, this, &CellInfoWidget::onStopButtonClicked);
}

CellInfoWidget::~CellInfoWidget()
{
    delete ui;
}

void CellInfoWidget::setProfileName(QString name)
{
    ui->profileNameLabel->setText(name);
}

void CellInfoWidget::started()
{
    ui->progressBar->setEnabled(true);
    ui->consoleToolButton->setEnabled(true);
    ui->pauseToolButton->setEnabled(true);
    ui->stopToolButton->setEnabled(true);
}

void CellInfoWidget::scheduled()
{
    ui->progressBar->setEnabled(false);
    ui->consoleToolButton->setEnabled(false);
    ui->pauseToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
}

void CellInfoWidget::updateProcess(QString data, int type)
{
    Q_UNUSED(type);
    ui->processLabel->setText(data);
}

void CellInfoWidget::updateProgress(int num)
{
    ui->progressBar->setValue(num);
}

void CellInfoWidget::updateErrorProgress(int num)
{
    QPalette p = ui->progressBar->palette();
    p.setColor(QPalette::Highlight, Qt::darkRed);
    ui->progressBar->setPalette(p);
    ui->progressBar->repaint();
    ui->progressBar->setValue(num);
}

void CellInfoWidget::updateErrorText(QString message)
{
    ui->processLabel->setText(message);
}

void CellInfoWidget::setTimerEnd(QString date)
{
    ui->processLabel->setText(date);
}

void CellInfoWidget::setProgressminmax(int start, int end)
{
    ui->progressBar->setRange(start, end);
}

void CellInfoWidget::onConsoleButtonClicked()
{
    consoleButtonClicked(this->objectName());
}

void CellInfoWidget::onPauseButtonClicked()
{
    pauseButtonClicked(this->objectName());
}

void CellInfoWidget::onStopButtonClicked()
{
    stopButtonClicked(this->objectName());
}
