#include "filetextreplace.h"

int FileTextReplace::functionMain(int argc, QStringList *args)
{
    if(argc < 4){
        setErrorMessage(tr("Argument is missing"));
        return -1;
    }

    QString input = args->at(0);
    QString output = args->at(1);
    QFile res(input);

    if(!res.open(QIODevice::Text | QIODevice::ReadOnly)) return -1;

    QString tmpstr;

    QRegExp re = QRegExp(args->at(2));

    QTextStream in(&res);
    tmpstr = in.readAll();

    res.close();
    re.indexIn(tmpstr);
    QString repl = re.capturedTexts().at(0);

    tmpstr = tmpstr.replace(repl, args->at(3));

    writeToText(output, tmpstr);

    return 0;
}

int FileTextReplace::launchSettingWidget(QStringList *currentargs, QStringList *resultargs
                                         , QPoint parentpos, QString parentstylesheet)
{
    FileTextReplaceDialog *tdialog = new FileTextReplaceDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}

bool FileTextReplace::writeToText(QString filename, QString text)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}
