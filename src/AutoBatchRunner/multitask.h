#ifndef MULTITASK_H
#define MULTITASK_H


class MultiTask : public QObject
{
    Q_OBJECT
public:
    explicit MultiTask(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MULTITASK_H