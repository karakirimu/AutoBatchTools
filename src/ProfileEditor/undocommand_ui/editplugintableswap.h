#ifndef PLUGINTABLESWAPOPERATION_H
#define PLUGINTABLESWAPOPERATION_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditPluginTableSwap : public QUndoCommand
{
public:
    EditPluginTableSwap(const int &targetindex
                             , const int &tablebefore
                             , const int &tableafter
                             , QList<QList<QStringList> *> *cache
                             , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    int m_targetindex;
    int m_indexbefore;
    int m_indexafter;
    QString m_objname;
    QList<QList<QStringList> *> *m_cache;

    int SKIP;
};

#endif // PLUGINTABLESWAPOPERATION_H
