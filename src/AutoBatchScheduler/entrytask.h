#ifndef ENTRYTASK_H
#define ENTRYTASK_H

#include <QObject>

class EntryTask : public QObject
{
    Q_OBJECT
public:
    explicit EntryTask(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ENTRYTASK_H