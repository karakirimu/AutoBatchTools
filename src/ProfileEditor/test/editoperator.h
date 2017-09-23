#ifndef EDITFILEOPERATOR_H
#define EDITFILEOPERATOR_H

#include <QEvent>
#include <QDateTime>
#include <QFileInfo>
#include <QObject>
#include <QUndoCommand>
#include <QApplication>
#include <editfullcommand.h>
#include <addcommand.h>
#include <deletecommand.h>
#include <editcommand.h>
#include <insertcommand.h>
#include <QClipboard>
#include <../processxmlbuilder/processxmlbuilder.h>
#include "listmimedata.h"

#define XML_MIN 2

class EditOperator : public QObject
{
    Q_OBJECT
public:
    explicit EditOperator(QObject *parent = nullptr);
    ~EditOperator();

    //data edited or not
    bool isEdited();
    QList<QStringList> read(int id);

    //editor editindex code (not contain tables)
    enum {
        NORMAL_TCHECK,NORMAL_TIMEOUT,NORMAL_ONLY
    };

signals:
    void loadfileChanged(QString);
    void edited(bool);
    void editindexUpdate(int);

public slots:
    //operate functions
    void addAction(QList<QStringList> xmlstruct);
    void insertAction(int id, QList<QStringList> xmlstruct);
    void deleteAction(int id);
    void editAction(int id, int innerid, int editcode, QList<QStringList> xmlstruct);
    void editFullAction(int id, QList<QStringList> xmlstruct);
    void cutAction(int id);
    void copyAction(int id);
    void pasteAction(int id);

    //not relation undo stack
    void newAction();
    void openAction(QString filepath);
    void saveAction(QString filepath);
    void exportAction(QString filepath);
    void abortAction();

    void loadcache(int amount);

protected:
    void timerEvent(QTimerEvent *event);

private:
    void save();
    void reset();

    //cache data
    QList<QList<QStringList>> *cache;

    //undo stack
    QUndoStack *undostack;

    //for timerEvent;
    int timerid;

    //autosavefile
    QString autosavefile;

    //loaded
    QString loadfile;
    int loadedxmlid = 0;
};

#endif // EDITFILEOPERATOR_H
