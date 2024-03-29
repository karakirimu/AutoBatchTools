/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITFILEOPERATOR_H
#define EDITFILEOPERATOR_H

#include <QEvent>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QUndoCommand>
#include <QApplication>
#include <QClipboard>
#include <QMainWindow>

#include <listmimedata.h>

#include <editorcachelist.h>

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
#include <settingconstant.h>

//table operation
#include <swapexectable.h>
#include <swapplugintable.h>
#include <editexectable.h>
#include <editplugintable.h>
#include <dragdropexectable.h>
#include <dragdropplugintable.h>

//tree variable operation
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
#include <stringbasepath.h>

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
    ~EditOperator() override;

    bool read(int id, EditorCache *list);
    void readAll(QList<EditorCache> *list);

    QUndoStack *getUndostack() const;
    int getCacheSize() const;
    QString getLoadfile() const;

    int getCurrentCommandType();

    //gui functions
    QRect getMainWindowGeometry();

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

signals:
    void loadfileChanged(QString);

    // reflect user 'edit' operation (argument is edit index)
    // It is necessary to implement it if it relates to flowtable
    void editUpdate(int);

    //Communication from AutoBatchEditor or flowtable widget to editortab
    void processIndexUpdate(int after, int before, int function);

public slots:
    //operate main process editor functions
    void addAction();
    void insertAction(int index, EditorCache *editorcache);
    void deleteAction(int id);
    void newAction();
    void autoSaveRecoveryAction(QString filepath);
    void openAction(QString filepath);
    void saveAction(QString filepath);
    void exportAction(QString filepath);
    void abortAction();

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
    void tableEditExecAction(int id, int tableid, QString newstr, int operation);
    void tableEditPluginAction(int id, int tableid, QString newstr, int operation);
    void tableEditPluginAction(int id, QStringList newstrlist, int operation);
    void tableDragDropExecAction(int id, QList<int> beforeid, int afterid);
    void tableDragDropPluginAction(int id, QList<int> beforeid, int afterid);

    //tree operation
    void treeEditGVariantAction(int id, QStringList variants, int operation);
    void treeSwapGVariantAction(int beforeid, int afterid);

    void treeEditLVariantAction(int id
                                , QStringList variants
                                , int operation
                                , int localindex);
    void treeSwapLVariantAction(int beforeid, int afterid, int localindex = 1);

    //textedit lineedit operation
    void textFileOutputAction(int id, QString newstr);
    void textProjectAuthorAction(int id, QString newstr);
    void textProjectDescriptAction(int id, QString newstr);
    void textProjectNameAction(int id, QString newstr);
    void textProjectVerAction(int id, QString newstr);
    void textSearchSeparateAction(int id, QString newstr);
    void textBasePathAction(int id, QString newstr);

    //process functions
    void copyAction(int id);
    void cutAction(int id);
    void pasteAction(int id);
    void swapAction(int before, int after);
    void dragDropAction(QList<int> before, int after);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void save();
    void reset();

    //internal function
    void loadcache(int amount);

    // Thread safe cache
    EditorCacheList *editorCache;

    //undo stack
    QUndoStack *undostack;

    //for timerEvent;
    int timerid;

    //autosavefile
    QString autosavefile;

    //loaded
    QString loadfile;

    //parent geometry
    QMainWindow *parentwid;

    //Setting ini constant
    SettingConstant sc;
};

#endif // EDITFILEOPERATOR_H
