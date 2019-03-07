#include "bufferedcopy.h"

int BufferedCopy::functionMain(int argc, QStringList *args)
{
    if(argc < 2){
        setErrorMessage(tr("Argument is missing"));
        return -1;
    }

    QString source = args->at(0);
    QString dest = args->at(1);
    QString filename = "";
    long cachesize = 524288;
    bool verify = false;

    //get argument data
    QString argcheck;
    for(int i = 2; i < argc; i++){
        argcheck = static_cast<QString>(args->at(i)).toLower();

        if(argcheck == "-rename" && (i + 1) < argc){
            filename = args->at(i + 1);
            i++;
        }

        if(argcheck == "-cs" && (i + 1) < argc){
            bool ok = false;
            int chsize = static_cast<QString>(args->at(i + 1)).toInt(&ok);
            if(ok) cachesize = chsize;
            i++;
        }

        if(argcheck == "-v") verify = true;
    }

    //output filename change
    QFileInfo stmp(source);
    if(filename == "" && stmp.exists()){
        dest = QFileInfo(dest).canonicalPath() \
                + stmp.fileName();
    }else if(stmp.exists()){
        dest = QFileInfo(dest).canonicalPath() \
                + filename;
    }else{
        setErrorMessage(tr("Input file does not exist"));
        return -1;
    }

    //execute copyprocess
    if(!copyData(source, dest, cachesize)){
        // copy failed
        return -1;
    }

    //in the case of the "verify" option is not enabled.
    if(!verify){
        setSuccessMessage(tr("Data writing completed !"));
        return 0;
    }

    if(!checkHash(source,dest)) return -1;

    return 0;
}

int BufferedCopy::launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
                                      , QPoint parentpos, QString parentstylesheet)
{
    BufferedCopyDialog *tdialog = new BufferedCopyDialog(currentargs);
    tdialog->move(parentpos - tdialog->rect().center());
    tdialog->setStyleSheet(parentstylesheet);

    if(tdialog->exec() == QDialog::Accepted){
        *resultargs = tdialog->getargs();
    }

    delete tdialog;
    return 0;
}

bool BufferedCopy::copyData(QString source, QString dest, long cachesize)
{
    //create destination file
    QFile ofile(dest);
    if (!ofile.open(QIODevice::WriteOnly)){
        setErrorMessage(tr("Output file could not open"));
        return false;
    }

    //read source data
    QFile sfile(source);
    if (!sfile.open(QIODevice::ReadOnly)){
        setErrorMessage(tr("Input file could not open"));
        ofile.close();
        return false;
    }

    //copy data and show progress
    QByteArray buffer;

    while(!sfile.atEnd()){
        //set buffer size
        buffer = sfile.read(cachesize);
        ofile.write(buffer);
    }
    sfile.close();
    ofile.close();

    return true;
}

bool BufferedCopy::checkHash(QString source, QString dest)
{
    QByteArray shash = sha1Hash(source).toHex();
    QByteArray dhash = sha1Hash(dest).toHex();

    setErrorMessage(tr("source       : ") + QString(shash) + tr("\r\n") + \
                    tr("[%1] destination : ").arg(this->metaObject()->className()) + \
                    QString(dhash));
    setSuccessMessage(tr("source       : ") + QString(shash) + tr("\r\n") + \
                    tr("[%1] destination : ").arg(this->metaObject()->className()) + \
                    QString(dhash));

    return (shash == dhash)? true : false;
}

QByteArray BufferedCopy::sha1Hash(QString file)
{
    QFile sf(file);
    if (sf.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        if (hash.addData(&sf)) {
            return hash.result();
        }
    }

    return QByteArray();
}
