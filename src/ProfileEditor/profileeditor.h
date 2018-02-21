#ifndef PROFILEEDITOR_H
#define PROFILEEDITOR_H

#include "runtasksignalbinder.h"

#include <QMainWindow>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
#include <SettingDialog.h>
#include <editoperator.h>

namespace Ui {
class ProfileEditor;
}

class ProfileEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileEditor(QWidget *parent = 0);
    explicit ProfileEditor(QStringList loadfile, QWidget *parent = 0);
    ~ProfileEditor();

    void closeEvent(QCloseEvent *event);

signals:
    void rowPosChanged(QString);

private slots:
    //file
    void newfileAction();
    void openAction();
    void saveAction();
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

    void launchSettingAction();
    void themeChangeAction();

    void onTitleChanged(QString newload);
    void onFileEdited(bool edited);
    void itemChangedAction(int index);

    //tree acesss
    void setTreerowpos_select(int value, int from);
    void setTreerowpos_update(int after, int before, int function, int sendfrom);

    //undo,redo command
    void onUndoTextChanged(QString text);
    void onRedoTextChanged(QString text);

    void about();

private:

    void setLoadfile(const QString &value);
    void initStatusBar();

    //reset ui
//    void preResetUi();
    void postResetUi();

    Ui::ProfileEditor *ui;
    SettingDialog *settingdialog;
    QFileDialog *fdialog;
    int rowpos;
    QString loadfile;
    bool lastedited = false;

    EditOperator *editop;
    RunTaskSignalBinder *rbinder;

};

#endif // PROFILEEDITOR_H
