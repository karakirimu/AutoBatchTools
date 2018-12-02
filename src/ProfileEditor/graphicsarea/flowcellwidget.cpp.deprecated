#include "flowcellwidget.h"
#include "ui_flowcellwidget.h"

FlowCellWidget::FlowCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlowCellWidget)
{
    ui->setupUi(this);

    //set default color
    framecolor = "background-color: transparent;";

    //set arrow icon
    arrowpixmap = QIcon(":/default_icons/flowarrow.png").pixmap(32,32);
    QPixmap icon(32,32);
    icon.fill(Qt::transparent);
    blankpixmap = icon;
//    QSettings settings( "./settings.ini", QSettings::IniFormat );

//    //theme settings
//    settings.beginGroup("pe_general");
//    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
//    settings.endGroup();

//    if(stylecolor == "Light"){
//        arrowpixmap = QIcon(":/default_icons/flowarrow.png").pixmap(32,32);
//    }

//    if(stylecolor == "Dark"){
//        arrowpixmap = QIcon(":/icons/downtriangle_light.png").pixmap(32,32);
//    }
}

FlowCellWidget::~FlowCellWidget()
{
    delete ui;
}

void FlowCellWidget::setTypeAll(const QString type, const QPixmap *pixmap, \
                                const QString *colorCssText, const QString *frameCssText)
{
    ui->typelabel->setText("<b>" + type + "</b>");
    ui->pixlabel->setPixmap(*pixmap);

    ui->pixlabel->setStyleSheet(*colorCssText);
    ui->typelabel->setStyleSheet(*colorCssText);
    ui->textEdit->setStyleSheet(*colorCssText);

    framecolor = *frameCssText;
    ui->innerFrame->setStyleSheet(*frameCssText);
}

//void FlowCellWidget::setType(const QString type)
//{
//    ui->typelabel->setText("<b>" + type + "</b>");
//}

//void FlowCellWidget::setTypepixmap(const QPixmap *pixmap)
//{
//    ui->pixlabel->setPixmap(*pixmap);
//}

//void FlowCellWidget::setLabelcolor(const QString *cssText)
//{
//    ui->pixlabel->setStyleSheet(*cssText);
//    ui->typelabel->setStyleSheet(*cssText);
//    ui->textBrowser->setStyleSheet(*cssText);
//}

//void FlowCellWidget::setFramecolor(const QString *cssText)
//{
//    framecolor = *cssText;
//    ui->innerFrame->setStyleSheet(*cssText);
//}

void FlowCellWidget::setContent(QString htmlText)
{
    ui->textEdit->setHtml(htmlText);
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
    ui->arrowBottom->setPixmap(blankpixmap);
}

void FlowCellWidget::showArrow()
{
    ui->arrowBottom->setPixmap(arrowpixmap);
}

void FlowCellWidget::selectedItem()
{
    ui->innerFrame->setStyleSheet("background-color: #34504f;");
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
