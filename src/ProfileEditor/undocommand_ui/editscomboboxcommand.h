#ifndef EDITSCOMBOBOXCOMMAND_H
#define EDITSCOMBOBOXCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

//for search combobox
class EditScomboBoxCommand : public QUndoCommand
{
public:
    EditScomboBoxCommand(const int &targetindex
                         , const QString &newstring
                         , const int &newsearchindex
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int m_targetindex;
    QString m_oldstring;
    QString m_newstring;
    int m_oldindex;
    int m_newindex;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;
    int m_indexpos = 0;
};

#endif // EDITSCOMBOBOXCOMMAND_H