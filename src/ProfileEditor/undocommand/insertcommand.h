#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include <QHash>
#include <QUndoCommand>

class InsertCommand : public QUndoCommand
{
public:
    InsertCommand(const int &targetindex
                  , const QList<QStringList> &inserted
                  , QList<QList<QStringList>> *cache
                  , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int m_targetindex;
    QList<QStringList> m_inserted;
    QList<QList<QStringList>> *m_cache;
};

#endif // INSERTCOMMAND_H
