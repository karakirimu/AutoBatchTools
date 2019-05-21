#include "sleep.h"

int Sleep::functionMain(int argc, QStringList *args)
{
    if(argc < 1){
        setErrorMessage(tr("Argument is missing"));
        return -1;
    }

    int time = (args->at(0)).toInt();

    QEventLoop loop;
    QTimer::singleShot( time, &loop, SLOT(quit()));
    loop.exec();

    return 0;
}

int Sleep::launchSettingWidget(QStringList *currentargs, QStringList *resultargs, QPoint parentpos, QString parentstylesheet)
{
    SleepDialog *tdialog = new SleepDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}
