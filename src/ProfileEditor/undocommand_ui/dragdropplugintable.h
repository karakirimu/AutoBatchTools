#ifndef DRAGDROPPLUGINTABLE_H
#define DRAGDROPPLUGINTABLE_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class DragDropPluginTable : public QUndoCommand
{
public:
    DragDropPluginTable(const int &targetindex
                        , const QList<int> tablebefore
                        , const int &tableafter
                        , QList<QList<QStringList> *> *cache
                        , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    void updateIndex(int count);

    int m_targetindex;
    QList<int> m_indexbefore;
    int m_indexafter;

    QHash<int, QString> m_oldstr;
    QString m_objname;

    QList<QList<QStringList> *> *m_cache;

    ProcessXmlListGenerator pxlg;

    int SKIP;

};

#endif // DRAGDROPPLUGINTABLE_H
