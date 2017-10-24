#include "flowcellwidget.h"
#include "ui_flowcellwidget.h"

FlowCellWidget::FlowCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlowCellWidget)
{
    ui->setupUi(this);

    //set default color
    framecolor = "background-color: transparent;";
    arrowpixmap = QIcon(":/icons/downtriangle.png").pixmap(32,32);
}

FlowCellWidget::~FlowCellWidget()
{
    delete ui;
}

void FlowCellWidget::setType(QString type)
{
    ui->typelabel->setText(type);
}

void FlowCellWidget::setTypepixmap(QPixmap pixmap)
{
    ui->pixlabel->setPixmap(pixmap);
}

void FlowCellWidget::setTypecolor(QString cssText)
{
    ui->pixlabel->setStyleSheet(cssText);
    ui->typelabel->setStyleSheet(cssText);
    ui->textBrowser->setStyleSheet(cssText);
}

void FlowCellWidget::setFramecolor(QString cssText)
{
    framecolor = cssText;
    ui->innerFrame->setStyleSheet(cssText);
}

void FlowCellWidget::setContent(QString htmlText)
{
    ui->textBrowser->setHtml(htmlText);
}

//void FlowCellWidget::setCellData(FlowCellData data)
//{
//    celldata = data;
//    ui->typelabel->setText(data.getType());
//    ui->typelabel->setPixmap(data.getTypepixmap());
//    ui->typelabel->setStyleSheet(data.getTypecolor());
//    ui->textBrowser->setStyleSheet(data.getTypecolor());
//    ui->innerFrame->setStyleSheet(data.getFramecolor());
//}

void FlowCellWidget::hideArrow()
{
//    this->resize(this->width(), this->height() - ui->arrowBottom->height());
    QPixmap icon(32,32);
    icon.fill(Qt::transparent);
    ui->arrowBottom->setPixmap(icon);
}

void FlowCellWidget::showArrow()
{
    ui->arrowBottom->setPixmap(arrowpixmap);
}

void FlowCellWidget::selectedItem()
{
    ui->innerFrame->setStyleSheet("background-color: rgb(159, 179, 211);");
}

void FlowCellWidget::unSelectedItem()
{
    ui->innerFrame->setStyleSheet(framecolor);
}

//FlowCellData FlowCellWidget::getCelldata() const
//{
//    return celldata;
//}

//void FlowCellWidget::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::LeftButton){


//    QWidget::mousePressEvent(event);
//}
