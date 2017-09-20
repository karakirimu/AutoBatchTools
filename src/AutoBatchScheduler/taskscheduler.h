#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <QObject>

class TaskScheduler : public QObject
{
    Q_OBJECT
public:
    explicit TaskScheduler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TASKSCHEDULER_H