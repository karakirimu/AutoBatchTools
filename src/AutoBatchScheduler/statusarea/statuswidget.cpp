#include "statuswidget.h"
#include "ui_statuswidget.h"

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
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
