#ifndef XMLLISTGENERATOR_H
#define XMLLISTGENERATOR_H

#include <QObject>

class XmlListGenerator : public QObject
{
    Q_OBJECT
public:
    explicit XmlListGenerator(QObject *parent = nullptr);

signals:

public slots:
};

#endif // XMLLISTGENERATOR_H