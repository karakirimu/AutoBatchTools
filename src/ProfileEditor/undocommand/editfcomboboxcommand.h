#ifndef EDITFCOMBOBOXCOMMAND_H
#define EDITFCOMBOBOXCOMMAND_H

#include <QUndoCommand>
#include <xmllistgenerator.h>

class EditFComboBoxCommand : public QUndoCommand
{
public:
    EditFComboBoxCommand(const int &targetindex
//                         , const QString oldstring
                         , const QString newstring
//                         , const QString oldfile
                         , const QString newfile
                         , const QString &objname
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE;
    bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

    QString m_newstring;
    QString m_newfile;
private:
    int m_targetindex;
    QString m_oldstring;
    QString m_oldfile;
    QString m_objname;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;

};

#endif // EDITFCOMBOBOXCOMMAND_H
