#include "makedirectory.h"

int MakeDirectory::functionMain(int argc, QStringList *args)
{
    if(argc < 1){
        setErrorMessage(tr("Argument is missing"));
        return -1;
    }

    bool dirbool;
    QDir dir;
    dirbool = dir.mkpath(args->at(0));

    return dirbool ? 0 : -1;
}

int MakeDirectory::launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                                       , QPoint parentpos, QString parentstylesheet)
{
    Q_UNUSED(currentargs)
    Q_UNUSED(resultargs)
    Q_UNUSED(parentpos)
    Q_UNUSED(parentstylesheet)

    return 0;
}
