#ifndef COMBOPLUGINSELECT_H
#define COMBOPLUGINSELECT_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class ComboPluginSelect : public QUndoCommand
{
public:
    ComboPluginSelect(const int &targetindex
                      , const QString newstring
                      , const QString newfile
                      , QList<QList<QStringList> *> *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString m_newstring;
    QString m_newfile;
private:
    int m_targetindex;
    QString m_oldstring;
    QString m_oldfile;
    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> posinfo;

    ProcessXmlListGenerator pxlg;
};

#endif // COMBOPLUGINSELECT_H
