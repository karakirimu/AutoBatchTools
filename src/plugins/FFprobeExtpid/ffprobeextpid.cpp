#include "ffprobeextpid.h"

int FFprobeExtpid::functionMain(int argc, QStringList *args)
{
    if(argc < 3){
        setErrorMessage(tr("Argument is missing"));
        return -1;
    }

    QString proberesult = args->at(0);
    QFile res(proberesult);

    if(!res.open(QIODevice::Text | QIODevice::ReadOnly)) return -1;

    QString videopid = "";
    QString audiopid = "";
    QString tmparr;

    QRegExp re = QRegExp("(\\[0x[0-9][0-9].\\])");

    QTextStream in(&res);

    while(!in.atEnd()){
        tmparr = in.readLine();

        if(tmparr.contains("Video: mpeg2video")){
            re.indexIn(tmparr);

            // [0x...]
            tmparr = re.capturedTexts().at(0);

            // 0x...
            videopid.append(tmparr.mid(1,5));
            videopid.append("\n");
        }

        if(tmparr.contains("Audio: aac")){
            re.indexIn(tmparr);
            // [0x...]
            tmparr = re.capturedTexts().at(0);

            // 0x...
            audiopid.append(tmparr.mid(1,5));
            audiopid.append("\n");
        }
    }

    res.close();

    writeToText(args->at(1), videopid);
    writeToText(args->at(2), audiopid);
    return 0;
}

int FFprobeExtpid::launchSettingWidget(QStringList *currentargs, QStringList *resultargs
                                       , QPoint parentpos, QString parentstylesheet)
{
    FFprobeExtpidDialog *tdialog = new FFprobeExtpidDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}

bool FFprobeExtpid::writeToText(QString filename, QString text)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}
