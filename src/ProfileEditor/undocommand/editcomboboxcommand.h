#ifndef EDITCOMBOBOXCOMMAND_H
#define EDITCOMBOBOXCOMMAND_H

//for localvariant combobox
#include <QUndoCommand>
#include <xmllistgenerator.h>

class EditComboBoxCommand : public QUndoCommand
{
public:
    EditComboBoxCommand(const int &targetindex
//                        , const QString oldstring
                        , const QString newstring
                        , QList<QList<QStringList> *> *cache
                        , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int m_targetindex;
    QString m_oldstring;
    QString m_newstring;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;
};

#endif // EDITCOMBOBOXCOMMAND_H
