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

    //textに変更
    QFile gplfile(":/license/gpl-3.0-standalone.html");
    gplfile.open(QFile::ReadOnly);
    ui->gpl->setHtml(QString(gplfile.readAll()));
    gplfile.close();

    QFile lgplfile(":/license/lgpl-3.0-standalone.html");
    lgplfile.open(QFile::ReadOnly);
    ui->lgpl->setHtml(QString(lgplfile.readAll()));
    lgplfile.close();

}

AboutPE::~AboutPE()
{
    delete ui;
}
