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

#ifndef PROFILEEDITOR_H
#define PROFILEEDITOR_H

#ifndef Q_TEST_OBJECT
#define Q_TEST_OBJECT
#endif

#include <QMainWindow>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
#include <QProgressBar>

#include <settingdialog/settingdialog.h>
#include <editoperator.h>
#include <multitaskp.h>
#include <aboutpe.h>

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

    void closeEvent(QCloseEvent *event);

signals:
    void statusLabelChanged(QString);

private slots:
    //file
    void newfileAction();
    void openAction();
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

    void onTitleChanged(QString newload);
    void onFileEdited(bool edited);

    //item select change
    void itemChangedAction(int after, int before, int function, int sendfrom);

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

    //launch test setting in direct
    void testSettingTriggered();

    //text sender
    void updateRangeText(QString range);

    //popup
    void about();

private:
    void initStatusBar();
    void initUi();

    //run pause stop button selection
    void setRunButtonState(bool run, bool pause, bool stop);

    //overwrite save option (yes :0, no : 1, cancel : 2, not need to save : 3)
    int checkOverWrite();
    enum{YES,NO,CANCEL,NOTNEED};

    Ui::ProfileEditor *ui;

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

};

#endif // PROFILEEDITOR_H
