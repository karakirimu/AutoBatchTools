#ifndef EDITFULLCOMMAND_H
#define EDITFULLCOMMAND_H

#include <QHash>
#include <QUndoCommand>
#include <xmllistgenerator.h>

class EditFullCommand : public QUndoCommand
{
public:
    EditFullCommand(const int &targetindex
                    , QList<QStringList> *changed
                    , QList<QList<QStringList> *> *cache
                    , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE;
    bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

    QList<QStringList> *m_changed;

private:
    int m_targetindex;
    QList<QStringList> *m_before;
    QList<QList<QStringList> *> *m_cache;
    static const int MAGIC = 1;
};

#endif // EDITFULLCOMMAND_H
