#ifndef EDITTEXTCOMMAND_H
#define EDITTEXTCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditTextCommand : public QUndoCommand
{
public:
    EditTextCommand(const int &targetindex
//                    , const QString oldstring
                    , QString newstring
                    , const QString &objname
                    , QList<QList<QStringList> *> *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString m_newstring;
private:
    int m_targetindex;
    QString m_oldstring;
    QString m_objname;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;

};

#endif // EDITTEXTCOMMAND_H
