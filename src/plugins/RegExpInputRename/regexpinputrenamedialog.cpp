#include "regexpinputrenamedialog.h"
#include "ui_regexpinputrenamedialog.h"

RegExpInputRenameDialog::RegExpInputRenameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegExpInputRenameDialog)
{
    ui->setupUi(this);
}

RegExpInputRenameDialog::~RegExpInputRenameDialog()
{
    delete ui;
}
