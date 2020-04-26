#ifndef EDITORCACHEMANAGER_H
#define EDITORCACHEMANAGER_H

#include <QObject>

class EditorCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit EditorCacheManager(QObject *parent = nullptr);

signals:

};

#endif // EDITORCACHEMANAGER_H
