#ifndef DATACHANGECOMMAND_H
#define DATACHANGECOMMAND_H

#include <QHash>
#include <QUndoCommand>

//editid mapping indicates in editoperator
class EditCommand : public QUndoCommand
{
public:
    EditCommand(const int &targetindex
                      , const int &xmllistnum
                      , const int &editid
                      , const QStringList &after
                      , QList<QList<QStringList>> *cache
                      , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE;
    bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

    QStringList after() const;
private:
    int m_targetindex;
    int m_xmllistnum;
    QStringList m_before;
    QStringList m_after;
    int m_editid;
    QList<QList<QStringList>> *m_cache;

};

#endif // DATACHANGECOMMAND_H
