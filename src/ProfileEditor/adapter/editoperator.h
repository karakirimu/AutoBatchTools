/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

//checkbox operation
#include <checkeditallowinput.h>
#include <checkeditloopinf.h>
#include <checkeditsearchinput.h>
#include <checkedittimeout.h>
#include <checkeditrundetach.h>
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
#include <editsearchfileoutputtype.h>

//table operation
#include <swapexectable.h>
#include <swapplugintable.h>
#include <editexectable.h>
#include <editplugintable.h>
#include <dragdropexectable.h>
#include <dragdropplugintable.h>

//tree variant operation
#include <editglobalvartree.h>
#include <swapglobalvartree.h>

#include <editlocalvarianttree.h>
#include <swaplocalvarianttree.h>

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
#include <insertcommand.h>
#include <swapcommand.h>
#include <dragdropcommand.h>

//xml operation
#include <../processxmlbuilder/processxmlbuilder.h>

#define XML_MIN 2

class EditOperator : public QObject
{
    Q_OBJECT
public:
    explicit EditOperator(QObject *parent = nullptr);
    ~EditOperator();

    bool read(int id, QList<QStringList> *list);
    void readAll(QList<QList<QStringList> *> *list);
    //editor editindex code (not contain tables)

    QUndoStack *getUndostack() const;
    int getCacheSize() const;
    QString getLoadfile() const;

    int getCurrentCommandType();

    /**
     * @brief
     * Enum for propagating UI operations to other UI
     */
    enum {
        //! 0: Add action
        ADD,
        //! 1: Delete action
        DELETE,
        //! 2: Insert action
        INSERT,
        //! 3: Swap action
        SWAP,
        //! 4: Select action
        SELECT
    };

    //gui functions
    QRect getMainWindowGeometry();

signals:
    void loadfileChanged(QString);

    // reflect user 'edit' operation (argument is edit index)
    // It is necessary to implement it if it relates to flowtable
    void editUpdate(int);

    //Communication from profileeditor or flowtable widget to editortab
    void processIndexUpdate(int after, int before, int function);
//    void ui_scrollReachBottom();

public slots:
    //operate main process editor functions
    void addAction();
    void insertAction(int id, QList<QStringList> *xmlstruct);
    void deleteAction(int id);
    void newAction();
    void openAction(QString filepath);
    void saveAction(QString filepath);
    void exportAction(QString filepath);
    void abortAction();

//    void editAction(int id, int innerid, int editcode, QList<QStringList> xmlstruct);
    void editTabAction(int id, int newindex);

    //checkbox operation
    void checkAllowInputAction(int id, bool newcheck);
    void checkLoopInfAction(int id, bool newcheck);
    void checkSearchInputAction(int id, bool newcheck);

    void checkTimeoutAction(int id, bool newcheck);
    void checkRunDetachAction(int id, bool newcheck);
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
    void radioSearchFileOutputTypeAction(int id, int newvalue);

    //table operation
    void tableSwapExecAction(int id, int beforeid, int afterid);
    void tableSwapPluginAction(int id, int beforeid, int afterid);
    void tableEditVariantAction(int id, int tableid, QStringList variants, int operation);
    void tableEditExecAction(int id, int tableid, QString newstr, int operation);
    void tableEditPluginAction(int id, int tableid, QString newstr, int operation);
    void tableDragDropExecAction(int id, QList<int> beforeid, int afterid);
    void tableDragDropPluginAction(int id, QList<int> beforeid, int afterid);

    //tree operation
    void treeEditGVariantAction(int id, QStringList variants, int operation);
    void treeSwapGVariantAction(int beforeid, int afterid);

    void treeEditLVariantAction(int id, QStringList variants, int operation, int localindex);
    void treeSwapLVariantAction(int beforeid, int afterid, int localindex = 1);

    //textedit lineedit operation
    void textFileOutputAction(int id, QString newstr);
    void textProjectAuthorAction(int id, QString newstr);
    void textProjectDescriptAction(int id, QString newstr);
    void textProjectNameAction(int id, QString newstr);
    void textProjectVerAction(int id, QString newstr);
    void textSearchSeparateAction(int id, QString newstr);

    //process functions
    void copyAction(int id);
    void cutAction(int id);
    void pasteAction(int id);
    void swapAction(int before, int after);
    void dragDropAction(QList<int> before, int after);

    //check difference
    bool elementDiffCheck();

protected:
    void timerEvent(QTimerEvent *event);

//private slots:
//    void appendcache();

private:
    void save();
    void reset();

    //internal function
    void loadcache(int amount);

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
