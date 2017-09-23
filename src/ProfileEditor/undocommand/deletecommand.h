#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QHash>
#include <QUndoCommand>

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(const int &targetindex
                  , const QList<QStringList> &deleted
                  , QList<QList<QStringList>> *cache
                  , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int m_targetindex;
    QList<QStringList> m_deleted;
    QList<QList<QStringList>> *m_cache;
};

#endif // DELETECOMMAND_H
