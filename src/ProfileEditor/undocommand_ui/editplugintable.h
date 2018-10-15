#ifndef EXTRATABLECOMMAND_H
#define EXTRATABLECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditPluginTable : public QUndoCommand
{
public:
    EditPluginTable(const int &targetindex
                      ,const int &tableindex
                      ,QString newstr
                      ,const int operation
                      ,QList<QList<QStringList> *> *cache
                      ,QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString m_newstr;

private:
    void updateIndex(int count);
    void updateCounter(bool ascend);

    int m_targetindex;
    int m_tableindex;
    QString m_oldstr;
    int m_operation;
    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> posinfo;

    ProcessXmlListGenerator pxlg;

    int SKIP;
};

#endif // EXTRATABLECOMMAND_H
