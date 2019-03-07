#include "takelineplugin.h"

int TakeLinePlugin::functionMain(int argc, QStringList *args)
{
    if(argc != 3) return 1;

    QFile file(args->at(FILEORSTRING));
    int targetline = static_cast<QString>(args->at(LINENUM)).toInt();
    QString str;

    if(file.exists() && file.open(QFile::ReadOnly)){

        //file mode
        QTextStream in(&file);
        int i = 1;

        while(!in.atEnd()){
            str = in.readLine();
            if(i == targetline) break;
            if(i++ > targetline){
                str = "";
                break;
            }
        }

    }else{
        //string take mode
        QStringList data = static_cast<QString>(args->at(FILEORSTRING)).split('\n');
        if(data.count() < targetline){
            str = "";
        }else{
            targetline -= 1;
            str = data.at(targetline);
        }

    }

    if(str != ""){
        if(local->contains(args->at(VAR))) local->insert(args->at(VAR), str);
            setSuccessMessage(tr("Take line ") + QString::number(targetline) + tr("."));
        return 0;

    }else{
            setErrorMessage(tr("Can't take line."));
        return 1;

    }
}

int TakeLinePlugin::launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                                        , QPoint parentpos, QString parentstylesheet)
{
    TakeLineDialog *tdialog = new TakeLineDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}
