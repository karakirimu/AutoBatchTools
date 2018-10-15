#include "cellinfowidget.h"
#include "statuswidget.h"
#include "ui_statuswidget.h"

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

#if QT_VERSION < 0x050900
    //DEPRECATED:
    // exclude taskbar
    QRect rec = QApplication::desktop()->screenGeometry();

    // include taskbar
    QRect ava = QApplication::desktop()->availableGeometry();
#else
    //TODO:MAY CAUSE PROBLEM IN LINUX

    QList<QScreen *> scr = QGuiApplication::screens();
    // exclude taskbar
    QRect rec = scr.at(0)->geometry();

    // include taskbar
    QRect ava = scr.at(0)->availableGeometry();
#endif

#ifdef QT_DEBUG
    qDebug() << "height" << rec.height() << ava.height();
    qDebug() << "width" << rec.width() << ava.width();
    qDebug() << "y" << rec.y() << ava.y();
    qDebug() << "x" << rec.x() << ava.x();

#endif

    if(rec.height() > ava.height()){
        int taskbarsize = rec.height() - ava.height();
        if(rec.y() < ava.y()){
            //top
            this->move(rec.width() - this->width() - 30, taskbarsize + 10);

        }else{
            //bottom
            this->move(rec.width() - this->width() - 30, rec.height() - this->height() - taskbarsize - 10);

        }
    }else {

        int taskbarsize = rec.width() - ava.width();

        if(rec.x() < ava.x()){
            //left
            this->move(taskbarsize + 10, rec.height() - this->height() - 10);

        }else{
            //right
            this->move(rec.width() - this->width() - taskbarsize - 10, rec.height() - this->height() - 10);

        }
    }
}

void StatusWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
