#ifndef EDITFILEOPERATOR_H
#define EDITFILEOPERATOR_H

#include <QEvent>
#include <QDateTime>
#include <QFileInfo>
#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QUndoCommand>
#include <QApplication>
#include <QClipboard>
#include <QMainWindow>

#include <listmimedata.h>

#include <editlocalvartable.h>
#include <edittabcommand.h>
//#include <editvaluecommand.h>
//#include <edittextcommand.h>
//#include <editcheckcommand.h>
//#include <editfcomboboxcommand.h>
//#include <edittablecommand.h>

//checkbox operation
#include <checkeditallowinput.h>
#include <checkeditloopinf.h>
#include <checkeditsearchinput.h>
#include <checkedittimeout.h>
#include <checkonlyschedulernormal.h>
#include <checkonlyschedulerother.h>
#include <checkonlyschedulerplugin.h>
#include <checkonlyschedulersearch.h>

//combobox operation
#include <combopluginselect.h>
#include <comboprofileselect.h>
//old
#include <editcomboboxcommand.h>
#include <editscomboboxcommand.h>

//spinbox operation
#include <editlooparguments.h>
#include <editloopmax.h>
#include <editlooprecursive.h>
#include <edittimeout.h>

//radiobutton operation
#include <editsearchoutput.h>

//table operation
#include <swapexectable.h>
#include <swapplugintable.h>
//old
//#include <swaptablecommand.h>
#include <editexectable.h>
#include <editplugintable.h>

//textedit lineedit operation
#include <stringfileoutput.h>
#include <stringprojectauthor.h>
#include <stringprojectdescription.h>
#include <stringprojectname.h>
#include <stringprojectversion.h>
#include <stringsearchsep.h>

//direct data operation
#include <addcommand.h>
#include <deletecommand.h>
//#include <editcommand.h>
#include <insertcommand.h>
#include <swapcommand.h>

//xml operation
#include <../processxmlbuilder/processxmlbuilder.h>

#define XML_MIN 2

class EditOperator : public QObject
{
    Q_OBJECT
public:
    explicit EditOperator(QObject *parent = nullptr);
    ~EditOperator();

    //data edited or not
//    bool isEdited();

    bool read(int id, QList<QStringList> *list);
    void readAll(QList<QList<QStringList> *> *list);
    //editor editindex code (not contain tables)

    QUndoStack *getUndostack() const;
    int getCacheSize() const;

    int getCurrentCommandType();

    //detect called item
    enum{FLOWTABLE, MAINEDITOR};
    enum{ADD,DELETE,INSERT,SWAP,SELECT};

    //gui functions
    QRect getMainWindowGeometry();

    QString getLoadfile() const;

signals:
    void loadfileChanged(QString);

    // reflect user 'edit' operation (argument is edit index)
    void editUpdate(int);
    void ui_funcindexUpdate(int after, int before, int function, int sender);
//    void ui_scrollReachBottom();

public slots:
    //operate functions
    void addAction();
    void insertAction(int id, QList<QStringList> *xmlstruct);
    void deleteAction(int id);

//    void editAction(int id, int innerid, int editcode, QList<QStringList> xmlstruct);
    void editTabAction(int id, int newindex);

    //checkbox operation
    void checkAllowInputAction(int id, bool newcheck);
    void checkLoopInfAction(int id, bool newcheck);
    void checkSearchInputAction(int id, bool newcheck);

    void checkTimeoutAction(int id, bool newcheck);
    void checkOnlyNormalAction(int id, bool newcheck);
    void checkOnlySearchAction(int id, bool newcheck);
    void checkOnlyPluginAction(int id, bool newcheck);
    void checkOnlyOtherAction(int id, bool newcheck);

    //combobox operation
    void comboboxLocalValAction(int id, QString mnew);
    void comboboxSearchAction(int id, QString newstr, int newval);
    void comboboxPluginAction(int id, QString newstr, QString newfile);
    void comboboxProfileAction(int id, QString newstr, QString newfile);

    //spinbox operation
    void spinTimeoutAction(int id, int newvalue);
    void spinLoopMaxAction(int id, int newvalue);
    void spinLoopArgumentsAction(int id, int newvalue);
    void spinLoopRecursiveAction(int id, int newvalue);

    //radiobutton operation
    void radioSearchOutputAction(int id, int newvalue);

    //table operation
    void tableSwapExecAction(int id, int beforeid, int afterid);
    void tableSwapPluginAction(int id, int beforeid, int afterid);
    void tableEditVariantAction(int id, int tableid, QStringList variants, int operationt);
    void tableEditExecAction(int id, int tableid, QString newstr, int operation);
    void tableEditPluginAction(int id, int tableid, QString newstr, int operation);

    //textedit lineedit operation
    void textFileOutputAction(int id, QString newstr);
    void textProjectAuthorAction(int id, QString newstr);
    void textProjectDescriptAction(int id, QString newstr);
    void textProjectNameAction(int id, QString newstr);
    void textProjectVerAction(int id, QString newstr);
    void textSearchSeparateAction(int id, QString newstr);

    void copyAction(int id);
    void cutAction(int id);
    void pasteAction(int id);
    void swapAction(int before, int after);

    //not relation undo stack
    void newAction();
    void openAction(QString filepath);
    void saveAction(QString filepath);
    void exportAction(QString filepath);
    void abortAction();

    void loadcache(int amount);

protected:
    void timerEvent(QTimerEvent *event);

//private slots:
//    void appendcache();

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

    //parent geometry
    QMainWindow *parentwid;
};

#endif // EDITFILEOPERATOR_H
