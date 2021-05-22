/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROFILEEDITOR_H
#define PROFILEEDITOR_H

#ifndef Q_TEST_OBJECT
#define Q_TEST_OBJECT
#endif

#include "settingconstant.h"

#include <QMainWindow>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
#include <QProgressBar>
#include <QSignalMapper>
#include <QLineEdit>

#include <settingdialog/settingdialog.h>
#include <autosaveselectiondialog.h>
#include <editoperator.h>
#include <multitaskp.h>
#include <aboutabe.h>

namespace Ui {
class ProfileEditor;
}

class ProfileEditor : public QMainWindow
{
    Q_OBJECT
    Q_TEST_OBJECT

public:
    explicit ProfileEditor(QWidget *parent = nullptr);
    explicit ProfileEditor(QStringList loadfile, QWidget *parent = nullptr);
    ~ProfileEditor();

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void statusLabelChanged(QString);
    void openRecentClicked(QString);

private slots:
    //file
    void newfileAction();
    void openAction();
    void openRecentAction(QString filename);
    void clearOpenRecentAction();
    void openLastClosedAction();
    bool saveAction();
    void overWriteSaveAction();
    void exportAction();

    //Edit
    void undoAction();
    void redoAction();
    void addAction();
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void upAction();
    void downAction();
    void reloadAction();

    void launchSettingAction();
    void themeChangeAction();

    void reloadWindow(QString newload);
    void onFileEdited(bool edited);

    //item select change
    void itemChangedAction(int after, int before, int function);

    //undo,redo command
    void onUndoTextChanged(QString text);
    void onRedoTextChanged(QString text);

    //received from task
    void taskStarted(QString objectname, int runfrom);
    void taskPaused(QString objectname);
    void taskStopped(QString objectname);
    void taskEnd(QString objectname, int runfrom);

    //send to task
    void runTriggered();
    void pauseTriggered();
    void stopTriggered();

    //update hide
    void hideToolBarDesc(bool state);

    //launch test setting in direct
    void runTestSettingsTriggered();

    //text sender
    void updateRangeText(QString range);

    //popup
    void about();

    //initialize
    void initFileLoad();

private:
    // initialize
    void initRunToolBar();
    void initRunRangeToolBar();
    void initStatusBar();
    void initUi();

    // Open Recent menu actions
    void clearOpenRecent(bool deletelist);
    void setOpenRecent();

    //run pause stop button selection
    void setRunButtonState(bool run, bool pause, bool stop);

    /**
     * overwrite save option
     */
    enum {
        //! 0: yes
        YES,
        //! 1: no
        NO,
        //! 2: cancel
        CANCEL,
        //! 3: no need to save
        NOTNEED
    };

    int checkOverWrite();
    bool checkAutoSave();

    Ui::ProfileEditor *ui;

    // runtoolbar
    QToolBar *runToolBar;
    QAction *actionRunSetting;
    QAction *actionToolBarRun;

    // run test range toolbar
    QToolBar *testRangeToolBar;
    QLineEdit *rangeLineEdit;
    QAction *actionToolBarTestRange;

    //settindopen and save
    SettingDialog *settingdialog;
    QFileDialog *fdialog;

    //current data position
    int dataindexpos;

    //filename save
    bool lastedited = false;

    //function manage
    EditOperator *editop;

    //key is thread name
    MultiTaskP *mlTask;
    QString key;

    SettingConstant sc;

};

#endif // PROFILEEDITOR_H
