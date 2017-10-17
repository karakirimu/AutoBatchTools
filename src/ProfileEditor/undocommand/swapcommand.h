#ifndef SWAPCOMMAND_H
#define SWAPCOMMAND_H

#include <QHash>
#include <QUndoCommand>

class SwapCommand : public QUndoCommand
{
public:
    SwapCommand(const int &beforeindex
                , const int &afterindex
                , QList<QList<QStringList> *> *cache
                , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int m_beforeindex;
    int m_afterindex;
    QList<QStringList> m_added;
    QList<QList<QStringList> *> *m_cache;
};

#endif // SWAPCOMMAND_H
