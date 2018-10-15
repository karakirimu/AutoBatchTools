#ifndef CHECKONLYSCHEDULERPLUGIN_H
#define CHECKONLYSCHEDULERPLUGIN_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class CheckOnlySchedulerPlugin : public QUndoCommand
{
public:
    CheckOnlySchedulerPlugin(const int &targetindex
                             , const bool &newcheck
                             , QList<QList<QStringList> *> *cache
                             , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
//    int getxmlpos();

    int m_targetindex;
    QString m_oldcheck;
    QString m_newcheck;
    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> xmlpos;
    //init generator
    ProcessXmlListGenerator pxlg;

//    const static int SUBPOS = 3;
};

#endif // CHECKONLYSCHEDULERPLUGIN_H
