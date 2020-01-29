#ifndef EDITSEARCHFILEOUTPUTTYPE_H
#define EDITSEARCHFILEOUTPUTTYPE_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditSearchFileOutputType : public QUndoCommand
{
public:
    EditSearchFileOutputType(const int &targetindex
                             , int newvalue
                             , QList<QList<QStringList> *> *cache
                             , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    int m_newvalue;

private:
    int m_targetindex;
    int m_oldvalue;
    QList<QList<QStringList> *> *m_cache;
    ProcessXmlListGenerator pxlg;
};

#endif // EDITSEARCHFILEOUTPUTTYPE_H
