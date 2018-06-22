#include "cellinfowidget.h"
#include "statuswidget.h"
#include "ui_statuswidget.h"

#include <QItemDelegate>

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    installEventFilter(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);

    connect(ui->settingToolButton, &QToolButton::clicked, this, &StatusWidget::settingsClicked);

//    //test
//    CellInfoWidget *widget = new CellInfoWidget();
//    ui->enabledTableWidget->setRowCount(1);
//    //adjust row (test)
//    ui->enabledTableWidget->setRowHeight(0, 60);
//    ui->enabledTableWidget->setColumnCount(1);
//    ui->enabledTableWidget->setCellWidget(0,0,widget);
}

StatusWidget::~StatusWidget()
{ 
    delete ui;
}

void StatusWidget::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    ui->enabledTableWidget->setTaskSchedulerConnector(task);
}

void StatusWidget::showWidget()
{
    this->show();

    QRect rec = QApplication::desktop()->screenGeometry();
    this->move(rec.width() - this->width() - 30, rec.height() - this->height() - 70);
}

void StatusWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
