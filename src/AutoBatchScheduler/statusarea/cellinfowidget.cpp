#include "cellinfowidget.h"
#include "ui_cellinfowidget.h"

CellInfoWidget::CellInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cellInfoWidget)
{
    ui->setupUi(this);

    //init
    ui->progressBar->setValue(0);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    connect(ui->consoleToolButton, &QToolButton::clicked, this, &CellInfoWidget::onConsoleButtonClicked);
    connect(ui->pauseToolButton, &QToolButton::clicked, this, &CellInfoWidget::onPauseButtonClicked);
    connect(ui->stopToolButton, &QToolButton::clicked, this, &CellInfoWidget::onStopButtonClicked);

    connect(ui->profileCheckBox, &QCheckBox::toggled, this, &CellInfoWidget::onRunStatusChange);

    //hide console
    ui->consolearea->hide();

}

CellInfoWidget::~CellInfoWidget()
{
    delete ui;
}

int CellInfoWidget::indicateHeight()
{
    return ui->indicatearea->baseSize().height();
}

void CellInfoWidget::setConsoleTarget(TaskSchedulerConnector *taskc)
{
    ui->console->setTaskSchedulerConnector(taskc);
    ui->consolesender->setTaskSchedulerConnector(taskc);
    ui->console->setReadObjectName(this->objectName());
    ui->consolesender->setObjectName(this->objectName());
}

bool CellInfoWidget::isConsoleVisible()
{
    return ui->consolearea->isVisible();
}

void CellInfoWidget::setConsoleVisible(bool show)
{
    ui->consolearea->setVisible(show);
}

void CellInfoWidget::setProfileName(QString name)
{
    ui->profileCheckBox->setText(name);
}

void CellInfoWidget::started()
{
    ui->progressBar->setEnabled(true);
    ui->pauseToolButton->setEnabled(true);
    ui->stopToolButton->setEnabled(true);
}

void CellInfoWidget::scheduled()
{
    ui->progressBar->setEnabled(false);
    ui->pauseToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
}

void CellInfoWidget::updateProcess(QString data, int type)
{
    Q_UNUSED(type)
    ui->currentProcessLabel->setText(data);
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
    ui->currentProcessLabel->setText(message);
}

void CellInfoWidget::setRunStatus(bool enabled)
{
    ui->profileCheckBox->setChecked(enabled);
    ui->nextTimeLabel->clear();
}

void CellInfoWidget::onRunStatusChange(bool enabled)
{
    if(ui->stopToolButton->isEnabled()){
        //task is running
        disconnect(ui->profileCheckBox, &QCheckBox::toggled, this, &CellInfoWidget::onRunStatusChange);
        ui->profileCheckBox->setChecked(true);
        connect(ui->profileCheckBox, &QCheckBox::toggled, this, &CellInfoWidget::onRunStatusChange);

        //TEXT
        updateErrorText(tr("To turn off task, need to stop current task."));
    }else{
        //task is not running
        setRunStatus(enabled);
        emit changeRunStatus(enabled);
    }
}

void CellInfoWidget::setTimerEnd(QString date)
{
    ui->nextTimeLabel->setText(date);
}

void CellInfoWidget::setProgressminmax(int start, int end)
{
    ui->progressBar->setRange(start, end);
}

void CellInfoWidget::onConsoleButtonClicked()
{
    emit consoleButtonClicked(this->objectName());
}

void CellInfoWidget::onPauseButtonClicked()
{
    emit pauseButtonClicked(this->objectName());
}

void CellInfoWidget::onStopButtonClicked()
{
    emit stopButtonClicked(this->objectName());
}
