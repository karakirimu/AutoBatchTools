#include "aboutpe.h"
#include "ui_aboutpe.h"

#include <QFile>

AboutPE::AboutPE(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPE)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    QIcon icons(":/app_icons/app_pe.ico");
    ui->icon->setPixmap(icons.pixmap(128,128));

}

AboutPE::~AboutPE()
{
    delete ui;
}
