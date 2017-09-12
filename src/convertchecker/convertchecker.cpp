#include "convertchecker.h"


ConvertChecker::ConvertChecker(QObject *parent) : QObject(parent){}
ConvertChecker::~ConvertChecker(){}

int ConvertChecker::checkMPxmlDiff(QString file1, QString file2)
{
    QFile *f1 = new QFile(file1);
    QFile *f2 = new QFile(file2);

    if(!f1->open(QIODevice::ReadOnly | QFile::Text)){
        qDebug() << "can't read f1.";
        return -1;
    }

    if(!f2->open(QIODevice::ReadOnly | QFile::Text)){
        qDebug() << "can't read f2.";
        return -1;
    }

    QString read1 = f1->readAll();
    read1.remove(QRegularExpression("<uptime>(.+)?"));

    QString read2 = f2->readAll();
    read2.remove(QRegularExpression("<uptime>(.+)?"));

    f1->close();
    f2->close();

    delete f1;
    delete f2;

    return read1.compare(read2);
}
