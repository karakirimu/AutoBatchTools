#ifndef EDITFILEOPERATOR_H
#define EDITFILEOPERATOR_H

#include <QEvent>
#include <QDateTime>
#include <QFileInfo>
#include <QObject>
#include <QUndoCommand>
#include <QApplication>
#include <editfullcommand.h>
#include <edittabcommand.h>
#include <editvaluecommand.h>
#include <edittextcommand.h>
#include <editcheckcommand.h>
#include <editcomboboxcommand.h>
#include <editscomboboxcommand.h>
#include <editfcomboboxcommand.h>
#include <edittablecommand.h>
#include <addcommand.h>
#include <deletecommand.h>
#include <editcommand.h>
#include <insertcommand.h>
#include <swapcommand.h>
#include <swaptablecommand.h>
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

    bool read(int id, QList<QStringList> *list);
    //editor editindex code (not contain tables)
    enum {
        TABCHANGED,
        NORMAL_TCHECK,NORMAL_TIMEOUT,NORMAL_ONLY
    };

    QUndoStack *getUndostack() const;
    int getCacheSize() const;


    //detect called item
    enum{TREE, GRAPHICAREA, MAINEDITOR};
    enum{ADD,DELETE,INSERT,SWAP};

signals:
    void loadfileChanged(QString);
    void edited(bool);
    void editUpdate(int);
    void ui_selectindexUpdate(int, int);
    void ui_funcindexUpdate(int after, int before, int function, int semder);

public slots:
    //selected index signal
    void selectChange(int index, int from){emit ui_selectindexUpdate(index, from);}

    //operate functions
    void addAction();
    void insertAction(int id, QList<QStringList> *xmlstruct);
    void deleteAction(int id);

    void editAction(int id, int innerid, int editcode, QList<QStringList> xmlstruct);
    void editTabAction(int id, int newindex);
    void editTextAction(int id, QString mnew, QString obj);
    void editComboBoxAction(int id, QString mnew);
    void editFileComboAction(int id, QString newstr, QString newfile, QString obj);
    void editSearchComboAction(int id, QString newstr, int newval);
    void editValueAction(int id, int newval, QString obj);
    void editCheckAction(int id, bool newcheck, QString obj);
    void editFullAction(int id, QList<QStringList> *xmlstruct);
    void editTableAction(int id, int tableid, QString newstr, int operation, QString objname);

    void cutAction(int id);
    void copyAction(int id);
    void pasteAction(int id);
    void swapAction(int before, int after);
    void swapTableAction(int id, int beforeid, int afterid, QString objname);

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
    QList<QList<QStringList> *> *cache;

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
