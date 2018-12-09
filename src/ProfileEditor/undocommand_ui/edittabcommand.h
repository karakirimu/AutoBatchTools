#ifndef EDITTABCOMMAND_H
#define EDITTABCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditTabCommand : public QUndoCommand
{
public:
    EditTabCommand(const int &targetindex
                   , const int &newid
                   , QList<QList<QStringList> *> *cache
                   , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
//    void selectIndex(int index);

    int m_targetindex;
    int m_oldid;
//    QString m_oldauto;
    int m_newid;
//    QString m_newauto;
    QList<QList<QStringList> *> *m_cache;

    ProcessXmlListGenerator pxlg;

};

#endif // EDITTABCOMMAND_H
