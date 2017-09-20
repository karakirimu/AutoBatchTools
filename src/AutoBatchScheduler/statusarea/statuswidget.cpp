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

    QRect rec = QApplication::desktop()->screenGeometry();
    int d_height = rec.height();
    int d_width = rec.width();

    this->move(d_width - this->width() - 30, d_height - this->height() - 70);

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

void StatusWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
