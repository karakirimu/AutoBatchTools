#include "filetextreplacewidget.h"
#include "ui_filetextreplacewidget.h"

FileTextReplaceWidget::FileTextReplaceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTextReplaceWidget)
{
    ui->setupUi(this);
}

FileTextReplaceWidget::~FileTextReplaceWidget()
{
    delete ui;
}
