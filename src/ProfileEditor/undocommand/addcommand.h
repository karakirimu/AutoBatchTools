#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QHash>
#include <QUndoCommand>

class AddCommand : public QUndoCommand
{
public:
    AddCommand(const int &targetindex
               , QList<QStringList> *added
               , QList<QList<QStringList> *> *cache
               , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int m_targetindex;
    QList<QStringList> *m_added;
    QList<QStringList> m_addcopy;
    QList<QList<QStringList> *> *m_cache;
};

#endif // ADDCOMMAND_H
