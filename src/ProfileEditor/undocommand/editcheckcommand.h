#ifndef EDITCHECKCOMMAND_H
#define EDITCHECKCOMMAND_H

#include <QHash>
#include <QUndoCommand>
#include <xmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class EditCheckCommand : public QUndoCommand
{
public:
    EditCheckCommand(const int &targetindex
                     , const bool &newcheck
                     , const QString objname
                     , QList<QList<QStringList> *> *cache
                     , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    int getxmlpos();

    int m_targetindex;
    QString m_oldcheck;
    QString m_newcheck;
    QString m_objname;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> xmlpos;
};

#endif // EDITCHECKCOMMAND_H
