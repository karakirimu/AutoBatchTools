#include "regexpinputrename.h"

int RegExpInputRename::functionMain(int argc, QStringList *args)
{
    if(argc != 4) return -1;

    // argument 1 is file or dir
    QFileInfo finfo(args->at(0));

    // get file or basename
    QString base = finfo.baseName();

    // split argument 3 and 4


    // filename or basename replaces at 3 to 4

    // rename file or dir

    return 0;
}

int RegExpInputRename::launchSettingWidget(QStringList *currentargs, QStringList *resultargs, QPoint parentpos, QString parentstylesheet)
{
    RegExpInputRenameDialog *tdialog = new RegExpInputRenameDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}
