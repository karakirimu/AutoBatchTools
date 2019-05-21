#include "removedirectory.h"

int RemoveDirectory::functionMain(int argc, QStringList *args)
{
    if(argc < 1){
        setErrorMessage(tr("Argument is missing"));
        return -1;
    }

    bool dirbool;

    QFileInfo info(args->at(0));

    if(!(info.exists() && info.isDir())){
        setErrorMessage(tr("Input folder does not exist."));
        return -1;
    }

    QDir dir(info.absoluteFilePath());

    if(argc == 2 && args->at(1) == "-rf"){
        QDirIterator dit(dir.path(), QStringList() << "*", QDir::Files | QDir::NoSymLinks);

        while (dit.hasNext()){
            QFile::remove(dit.next());
        }
    }

    setErrorMessage("Input : " + info.absoluteFilePath());

    QDir dir2;

    dirbool = dir2.rmdir(info.absoluteFilePath());

    return dirbool ? 0 : -1;
}

int RemoveDirectory::launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                                         , QPoint parentpos, QString parentstylesheet)
{
    RemoveDirectoryDialog *tdialog = new RemoveDirectoryDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}
