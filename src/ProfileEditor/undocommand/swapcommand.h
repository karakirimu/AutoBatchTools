#ifndef SWAPCOMMAND_H
#define SWAPCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class SwapCommand : public QUndoCommand
{
public:
    SwapCommand(const int &beforeindex
                , const int &afterindex
                , QList<QList<QStringList> *> *cache
                , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int m_beforeindex;
    int m_afterindex;
    QList<QStringList> m_added;
    QList<QList<QStringList> *> *m_cache;
};

#endif // SWAPCOMMAND_H
