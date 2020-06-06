#include "regexpinputrenamewidget.h"
#include "ui_regexpinputrenamewidget.h"

RegExpInputRenameWidget::RegExpInputRenameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegExpInputRenameWidget)
{
    ui->setupUi(this);
}

RegExpInputRenameWidget::~RegExpInputRenameWidget()
{
    delete ui;
}
