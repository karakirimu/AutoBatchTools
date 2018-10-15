#ifndef EDITCOMBOBOXCOMMAND_H
#define EDITCOMBOBOXCOMMAND_H

//for localvariant combobox
#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditComboBoxCommand : public QUndoCommand
{
public:
    EditComboBoxCommand(const int &targetindex
//                        , const QString oldstring
                        , const QString newstring
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
    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> posinfo;
    ProcessXmlListGenerator pxlg;

};

#endif // EDITCOMBOBOXCOMMAND_H
