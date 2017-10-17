#ifndef EDITVALUECOMMAND_H
#define EDITVALUECOMMAND_H

#include <QUndoCommand>
#include <xmllistgenerator.h>

class EditValueCommand : public QUndoCommand
{
public:
    EditValueCommand(const int &targetindex
//                     , const int oldvalue
                     , int newvalue
                     , const QString &objname
                     , QList<QList<QStringList> *> *cache
                     , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE;
    bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

    int m_newvalue;

private:
    int m_targetindex;
    int m_oldvalue;
    QString m_objname;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;

};

#endif // EDITVALUECOMMAND_H
