#ifndef EDITSEARCHSEPARATOR_H
#define EDITSEARCHSEPARATOR_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class StringSearchSep : public QUndoCommand
{
public:
    StringSearchSep(const int &targetindex
                        , QString newstring
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
    ProcessXmlListGenerator pxlg;

};

#endif // EDITSEARCHSEPARATOR_H
