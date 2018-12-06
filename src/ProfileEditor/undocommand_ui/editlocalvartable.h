#ifndef EDITFULLCOMMAND_H
#define EDITFULLCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

/**
 * @brief The EditLocalVarTable class
 * newstrlist structure
 * 0       :1       :
 * variant :value   :
 */

class EditLocalVarTable : public QUndoCommand
{
public:
    EditLocalVarTable(const int &targetindex
                      , const int &tableindex
                      , QStringList newstrlist
                      , const int operation
                      , QList<QList<QStringList> *> *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QStringList m_newvar;
//    QString m_newval;

    int operation() const;
private:
//    void updateIndex(int count);
    void updateCounter(bool ascend);

    int m_targetindex;
    int m_tableindex;
    QStringList m_oldvar;
    int m_operation;
    QList<QList<QStringList> *> *m_cache;

    ProcessXmlListGenerator pxlg;

    int SKIP;
};

#endif // EDITFULLCOMMAND_H
