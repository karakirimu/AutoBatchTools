#ifndef CONVERTCHECKER_H
#define CONVERTCHECKER_H

#include <QObject>
#include <QFile>
#include <QIODevice>
#include <QRegularExpression>
#include <QDebug>
#include "convertchecker_global.h"

class CONVERTCHECKERSHARED_EXPORT ConvertChecker : public QObject
{
    Q_OBJECT
public:
    CONVERTCHECKERSHARED_EXPORT explicit ConvertChecker(QObject *parent = 0);
    CONVERTCHECKERSHARED_EXPORT ~ConvertChecker();
    CONVERTCHECKERSHARED_EXPORT int checkMPxmlDiff(QString file1, QString file2);
};

#endif // CONVERTCHECKER_H
